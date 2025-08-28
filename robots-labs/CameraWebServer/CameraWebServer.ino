#include "esp_camera.h"
#include "esp_http_server.h"
#include "esp_timer.h"
#include "Arduino.h"
#include "WiFi.h"
#include "ArduinoJson.h"
#include "algorithm"
#include "credentials.h"

// Pinos do ESP32-CAM
#define MOTOR_1A    2  // GPIO2
#define MOTOR_1B    14 // GPIO14 
#define MOTOR_2A    15 // GPIO15
#define MOTOR_2B    13 // GPIO13

// Configurações da câmera ESP32-CAM
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22



httpd_handle_t control_server = NULL;
httpd_handle_t stream_server = NULL;

void setupCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Erro ao inicializar camera: 0x%x", err);
        return;
    }
    
    sensor_t * s = esp_camera_sensor_get();
    s->set_framesize(s, FRAMESIZE_QVGA);
    s->set_quality(s, 12);
    
    Serial.println("Camera inicializada com sucesso");
}

void setupMotors() {
    pinMode(MOTOR_1A, OUTPUT);
    pinMode(MOTOR_1B, OUTPUT);
    pinMode(MOTOR_2A, OUTPUT);
    pinMode(MOTOR_2B, OUTPUT);
    
    digitalWrite(MOTOR_1A, LOW);
    digitalWrite(MOTOR_1B, LOW);
    digitalWrite(MOTOR_2A, LOW);
    digitalWrite(MOTOR_2B, LOW);
    
    Serial.println("Motores configurados");
}

void controlMotors(int motor1Speed, int motor2Speed) {
    Serial.printf("Controlando motores: M1=%d, M2=%d\n", motor1Speed, motor2Speed);
    
    // Motor 1
    if (motor1Speed >= 0) {
        digitalWrite(MOTOR_1A, HIGH);
        digitalWrite(MOTOR_1B, LOW);
    } else {
        digitalWrite(MOTOR_1A, LOW);
        digitalWrite(MOTOR_1B, HIGH);
    }

    // Motor 2
    if (motor2Speed >= 0) {
        digitalWrite(MOTOR_2A, HIGH);
        digitalWrite(MOTOR_2B, LOW);
    } else {
        digitalWrite(MOTOR_2A, LOW);
        digitalWrite(MOTOR_2B, HIGH);
    }
}

static esp_err_t streamHandler(httpd_req_t *req) {
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    static const char* STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=123456789000000000000987654321";
    static const char* STREAM_BOUNDARY = "\r\n--123456789000000000000987654321\r\n";
    static const char* STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

    httpd_resp_set_type(req, STREAM_CONTENT_TYPE);
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

    while(true) {
        fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Falha ao capturar camera");
            res = ESP_FAIL;
        } else {
            httpd_resp_send_chunk(req, STREAM_BOUNDARY, strlen(STREAM_BOUNDARY));
            
            char part_buf[64];
            size_t hlen = snprintf((char *)part_buf, 64, STREAM_PART, fb->len);
            httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
            httpd_resp_send_chunk(req, (const char *)fb->buf, fb->len);
            
            esp_camera_fb_return(fb);
            fb = NULL;
        }
        
        if(res != ESP_OK) {
            break;
        }
    }
    return res;
}

static esp_err_t controlHandler(httpd_req_t *req) {
    char content[100];
    size_t recv_size = std::min(req->content_len, sizeof(content) - 1);
    
    Serial.println("Recebendo comando de controle...");
    
    int ret = httpd_req_recv(req, content, recv_size);
    if (ret <= 0) {
        Serial.println("Erro ao receber dados");
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }
    
    content[recv_size] = '\0';
    Serial.printf("Dados recebidos: %s\n", content);
    
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, content);
    
    if (error) {
        Serial.println("Erro ao parsear JSON");
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "JSON inválido");
        return ESP_FAIL;
    }
    
    int motor1 = doc["motor1"] | 0;
    int motor2 = doc["motor2"] | 0;
    
    controlMotors(motor1, motor2);
    
    httpd_resp_set_type(req, "application/json");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_sendstr(req, "{\"status\":\"ok\"}");
    
    return ESP_OK;
}

void startControlServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;
    config.ctrl_port = 80;
    config.stack_size = 4096;
    config.max_uri_handlers = 2;
    
    if (httpd_start(&control_server, &config) == ESP_OK) {
        httpd_uri_t control_uri = {
            .uri       = "/control",
            .method    = HTTP_POST,
            .handler   = controlHandler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(control_server, &control_uri);
        Serial.println("Servidor de Controle iniciado na porta 80");
    }
}

void startStreamServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 81;
    config.ctrl_port = 81;
    config.stack_size = 8192;
    config.max_uri_handlers = 2;
    
    if (httpd_start(&stream_server, &config) == ESP_OK) {
        httpd_uri_t stream_uri = {
            .uri       = "/stream",
            .method    = HTTP_GET,
            .handler   = streamHandler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(stream_server, &stream_uri);
        Serial.println("Servidor de Stream iniciado na porta 81");
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n\n");
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    setupCamera();
    setupMotors();
    
    startControlServer();
    startStreamServer();
    
    Serial.println("Setup completo!");
}

void loop() {
    if (!control_server) {
        Serial.println("Reiniciando servidor de controle...");
        startControlServer();
    }
    
    if (!stream_server) {
        Serial.println("Reiniciando servidor de stream...");
        startStreamServer();
    }
    
    delay(1);
}