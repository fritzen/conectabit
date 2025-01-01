#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

#define ESP32_CAM

#define USMIN  600 
#define USMAX  2400

#define SERVO_1_RETRACT 2000
#define SERVO_1_EXPAND  600

#define SERVO_2_RETRACT 1900
#define SERVO_2_EXPAND  500

#define SERVO_3_RETRACT 1900
#define SERVO_3_EXPAND  500

#define SERVO_4_RETRACT 1900
#define SERVO_4_EXPAND  500

#define SERVO_1_CLIP 2000
#define SERVO_1_ROTATE  600

#define SERVO_2_CLIP 2000
#define SERVO_2_ROTATE  600

#define SERVO_3_CLIP 2000
#define SERVO_3_ROTATE  600

#define SERVO_4_CLIP 2000
#define SERVO_4_ROTATE  600


//
// WARNING!!! Make sure that you have either selected ESP32 Wrover Module,
//            or another board which has PSRAM enabled
//

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

const char* ssid = "ASUSAP";
const char* password = "lentilha";

IPAddress local_IP(192,168,0,2);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);


void startCameraServer();

void movimentar(uint8_t servonum, int us){
    servonum = servonum - 1; 
    pwm.writeMicroseconds(servonum, us); //600 expande - 2000
    delay(600);
}

void claw(uint8_t servonum, int us){
    servonum = servonum + 3; 
    pwm.writeMicroseconds(servonum, us); //600 expande - 2000
    delay(600);
}


void setup() {
  //Wire.begin(SDA_PIN, SCL_PIN);
  //Wire.begin(12,13);

  Serial.begin(115200);
  Serial.println("Magic Cube!");
  //pwm.begin(2, 14);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);  // The int.osc. is closer to 27MHz  
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(10);

      movimentar(1, SERVO_2_RETRACT);
      delay(1000);
      movimentar(2, SERVO_2_RETRACT);
      delay(1000);
      movimentar(3, SERVO_2_RETRACT);
      delay(1000);
      movimentar(4, SERVO_2_RETRACT);
      delay(1000);
      movimentar(1, SERVO_1_EXPAND);
      delay(1000);
      movimentar(2, SERVO_2_EXPAND);
      delay(1000);
      movimentar(3, SERVO_3_EXPAND);
      delay(1000);
      movimentar(4, SERVO_4_EXPAND);
      delay(1000);
  Serial.setDebugOutput(true);
  Serial.println();

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
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);//flip it back
    s->set_brightness(s, 1);//up the blightness just a bit
    s->set_saturation(s, -2);//lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

  WiFi.begin(ssid, password);



WiFi.mode(WIFI_AP);  

  WiFi.softAPConfig(local_IP, gateway, subnet); //? "Ready" : "Failed!"
  WiFi.softAP("ROBOT-NETWORK-2");

 Serial.println(WiFi.softAPIP());


  //while (WiFi.status() != WL_CONNECTED) {
   // delay(500);
   // Serial.print(".");
 // }
  
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
 // Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  // put your main code here, to run repeatedly:

   claw(2, SERVO_2_ROTATE);
      delay(1000);
      movimentar(2, SERVO_2_RETRACT);
      delay(1000);
      claw(2, SERVO_2_CLIP);
      delay(1000);
      movimentar(2, SERVO_1_EXPAND);
      delay(1000);

      claw(2, SERVO_1_CLIP);
      delay(100000);
      

}
