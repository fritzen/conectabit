#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h> // Websockets Lib, By Markus https://github.com/Links2004/arduinoWebSockets
#include <Hash.h>
#include <Servo.h>
#include "html.h" // Inclui o conteúdo do arquivo html.h

Servo myservo1;
Servo myservo2;

WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);
DNSServer dnsServer;

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);

const int pwmMotorA = 5;
const int pwmMotorB = 4;
const int dirMotorA = 0;
const int dirMotorB = 2;

uint8_t pin_led = 16;

IPAddress local_IP(192,168,0,1);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

bool isClaw = false;

void setup() {
  Serial.begin(9600);
  myservo1.attach(14);
  myservo2.attach(12);

  pinMode(pwmMotorA, OUTPUT);
  pinMode(pwmMotorB, OUTPUT);
  pinMode(dirMotorA, OUTPUT);
  pinMode(dirMotorB, OUTPUT);

  WiFi.mode(WIFI_AP);
 //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  //WiFi.softAP("DNSServer CaptivePortal example");
  
  WiFi.softAPConfig(local_IP, gateway, subnet);
  
  WiFi.softAP("ROBOT-NETWORK");

  dnsServer.start(DNS_PORT, "*", apIP);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  server.on("/", []() { server.send_P(200, "text/html", html); });
  server.onNotFound([]() {
    server.send(200, "text/html", html);
  });

  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webSocket.loop();
  server.handleClient();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    String txt = "";
    for (int i = 0; i < length; i++) {
      txt = txt + ((char)payload[i]);
    }

    if (txt == "G") {
      isClaw = !isClaw;
      DoClaw();
    } else {
      int delimiterIndex = txt.indexOf(',');

      if (delimiterIndex != -1) {
        String xStr = txt.substring(0, delimiterIndex);
        String yStr = txt.substring(delimiterIndex + 1);
        int x = xStr.toInt();
        int y = yStr.toInt();

        int speedA = map(x, -255, 255, -255, 255);
        int speedB = map(y, -255, 255, -255, 255);
        controlMotor(speedA, speedB);
      }
    }
  }
}

void controlMotor(int speedA, int speedB) {
  digitalWrite(dirMotorA, speedA >= 0 ? HIGH : LOW);
  digitalWrite(dirMotorB, speedB >= 0 ? HIGH : LOW);
  
  analogWrite(pwmMotorA, abs(speedA));
  analogWrite(pwmMotorB, abs(speedB));
}

void DoClaw() {
  if (isClaw) {
    OpenClaw();
  } else {
    CloseClaw();
  }
}

void OpenClaw() {
  myservo1.write(10);
  myservo2.write(0);
}

void CloseClaw() {
  myservo1.write(105);
  myservo2.write(180);
}