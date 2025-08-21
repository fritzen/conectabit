#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h> //Instale a biblioteca "WebSockets" por Markus Sattler
#include <Hash.h>

#include "captive_html.h" 
#include "joystick_html.h"

WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);
DNSServer dnsServer;

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);

// ESP12E: Shield com ponte H embutida (pinos obrigatórios)
const int pwmMotorA = 5;    // GPIO5 (D1) - PWM Motor A - OBRIGATÓRIO
const int pwmMotorB = 4;    // GPIO4 (D2) - PWM Motor B - OBRIGATÓRIO
const int dirMotorA = 0;    // GPIO0 (D3) - Direction Motor A - OBRIGATÓRIO  
const int dirMotorB = 2;    // GPIO2 (D4) - Direction Motor B - OBRIGATÓRIO

IPAddress local_IP(192,168,0,1);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

bool isClaw = false;

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  Serial.println("=== ESP12E MOTOR SHIELD MODE ===");
  Serial.println("Motor A PWM: GPIO5, Dir: GPIO0");
  Serial.println("Motor B PWM: GPIO4, Dir: GPIO2");

  // Configurar motores
  pinMode(pwmMotorA, OUTPUT);
  pinMode(pwmMotorB, OUTPUT);
  pinMode(dirMotorA, OUTPUT);
  pinMode(dirMotorB, OUTPUT);
  
  // Inicializar motores parados
  digitalWrite(dirMotorA, LOW);
  digitalWrite(dirMotorB, LOW);
  analogWrite(pwmMotorA, 0);
  analogWrite(pwmMotorB, 0);
  Serial.println("Motores inicializados");
  
  // Delay para estabilizar GPIO0
  delay(1000);

  // Configuração WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("ROBOT-NETWORK");

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Configuração WebSocket e servidor
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  server.on("/", []() { 
    server.send_P(200, "text/html", joystick_html); 
  });

  server.begin();
  Serial.println("Sistema pronto!");
}

void loop() {
  webSocket.loop();
  server.handleClient();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    String txt = "";
    for (int i = 0; i < length; i++) {
      txt = txt + ((char)payload[i]);
    }

    Serial.println("Received: " + txt);

    int delimiterIndex = txt.indexOf(',');

    if (delimiterIndex != -1) {
      String xStr = txt.substring(0, delimiterIndex);
      String yStr = txt.substring(delimiterIndex + 1);
      int x = xStr.toInt();
      int y = yStr.toInt();

      Serial.print("X: ");
      Serial.print(x);
      Serial.print(", Y: ");
      Serial.println(y);

      controlMotor(x, y);
    }
  }
}

void controlMotor(int x, int y) {
  // Mapear valores para PWM (0-1023 no ESP8266)
  int speedA = map(abs(x), 0, 255, 0, 1023);
  int speedB = map(abs(y), 0, 255, 0, 1023);

  // Definir direção baseada no sinal
  digitalWrite(dirMotorA, x >= 0 ? LOW : HIGH);
  digitalWrite(dirMotorB, y >= 0 ? LOW : HIGH);
  
  // Aplicar velocidade PWM
  analogWrite(pwmMotorA, speedA);
  analogWrite(pwmMotorB, speedB);

  Serial.print("Motor A - Speed: ");
  Serial.print(speedA);
  Serial.print(", Dir: ");
  Serial.print(x >= 0 ? "FWD" : "REV");
  Serial.print(" | Motor B - Speed: ");
  Serial.print(speedB);
  Serial.print(", Dir: ");
  Serial.println(y >= 0 ? "FWD" : "REV");
}