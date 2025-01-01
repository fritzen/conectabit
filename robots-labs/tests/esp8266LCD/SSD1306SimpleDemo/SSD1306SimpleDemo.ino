/**
 * The MIT License (MIT)
 * LIBS
 * https://thingpulse.com
 *
 */

 //https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/soft-access-point-class.html
 
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include "images.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <Servo.h>

#include "webpage.h"
#include "css.h"
#include "js.h"
#include "html.h"

WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);


#define in1 0
#define in2 3
#define in4 1
#define in3 2

uint8_t pin_led = 16;
char* ssid = "YOUR_SSID";
char* password = "YOUR_PASSWORD";

IPAddress local_IP(192,168,0,2);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

int delayTime = 0;

String command = "";

int pos = 0;    // variable to store the servo position

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h // Extra param required for 128x32 displays.

#define SETUP_DURATION 3000
#define SENSOR_THRESHOLD 390

#define AP_NAME "ROBOT-NETWORK-2"

typedef void (*Setup)(void);

int setupMode = 0;

bool linefollower = true;


#define LEFT_SENSOR_PIN 1
#define RIGHT_SENSOR_PIN 2

//0 = expectedRight
//1 = expectedLeft
//2 = nothing
int expected = 0;


short left_sensor;
short right_sensor;



void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(in3,OUTPUT); 

  pinMode(LEFT_SENSOR_PIN,INPUT);
  pinMode(RIGHT_SENSOR_PIN,INPUT);

  // Initialising the UI will init the display too.
  display.init();

  //display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  STOP();

  WiFi.mode(WIFI_AP);  
 
  WiFi.softAPConfig(local_IP, gateway, subnet); //? "Ready" : "Failed!"
  WiFi.softAP(AP_NAME);

  //Serial.println(WiFi.softAPIP());

  server.on("/nipplejs.js",[](){server.send_P(200,"application/javascript", js);}); 
  server.on("/styles.css",[](){server.send_P(200,"text/css", css);});
  server.on("/",[](){server.send_P(200,"text/html", html);});
  server.on("/joy2.html",[](){server.send_P(200,"text/html", webpage);});

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);    
  server.begin();

  

}

void drawAPIP() {
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, AP_NAME);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 12, "http://192.168.0.2");
    
}

void drawCircleSetup() {
  for (int i=1; i < 8; i++) {
    display.setColor(WHITE);
    display.drawCircle(32, 32, i*3);
    if (i % 2 == 0) {
      display.setColor(BLACK);
    }
    display.fillCircle(96, 32, 32 - i* 3);
  }
}



void drawWifi() {    
    display.drawXbm(34, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);    
}

void drawSorriso() {
     display.drawXbm(0, 0, 128, 32, BMP);
}

void drawPiscada() {
    display.drawXbm(0, 0, 128, 32, BMP2);
}

Setup setups[] = {drawWifi, drawAPIP};
int setupLength = (sizeof(setups) / sizeof(Setup));
long timeSinceLastModeSwitch = 0;

int tempoSorriso = 0;

void loop() {
  
  delay(1);
  
  if (WiFi.softAPgetStationNum() == 0){
    display.clear();
    setups[setupMode]();
    if (millis() - timeSinceLastModeSwitch > SETUP_DURATION) {
      setupMode = (setupMode + 1)  % setupLength;
      timeSinceLastModeSwitch = millis();
    }
    display.display(); 
  } else {
    delay(1);
    display.clear();
    tempoSorriso++;
    if (tempoSorriso > 0 && tempoSorriso < 3000) {
        drawSorriso();  
    } else {
      drawPiscada();
      if (tempoSorriso > 3500) {
        tempoSorriso = 0;
      }
    }
    display.display(); 
  }

  LineFollower();

  webSocket.loop();
  server.handleClient();
}


void readSensors() {
    left_sensor = analogRead(LEFT_SENSOR_PIN);
    right_sensor = analogRead(RIGHT_SENSOR_PIN);
    
     Serial.print(left_sensor);
     Serial.print("x");
     Serial.print(right_sensor); 
     Serial.print("*");

    if (left_sensor < SENSOR_THRESHOLD) {
      left_sensor = BLACK;
    } else {
      left_sensor = WHITE;
    }

    if (right_sensor < SENSOR_THRESHOLD) {
      right_sensor = BLACK;
    } else {
      right_sensor = WHITE;
    }    


//    Serial.print(left_sensor);
//    Serial.print("x");
//    Serial.print(right_sensor); 
//    Serial.print("*"); 
//    Serial.print(distance);
//    Serial.println(" cm");
  
 }

void LineFollower(){
    readSensors();
      
    if (left_sensor == BLACK && right_sensor == WHITE){
        Serial.println("left_sensor");
        expected = 1;
        RIGHT_UP();
    } else if (left_sensor == WHITE && right_sensor == BLACK)  {
        Serial.println("right_sensor");
        expected = 0;
        LEFT_UP();
     } else {
        Serial.println("FORWARD");
        if (expected == 1 && right_sensor == BLACK || expected == 0 && left_sensor == BLACK )  {
          expected = 2; 
        }        
        if (expected == 1) {
          RIGHT();
        } else if (expected == 0) {
          LEFT();          
        } else if (left_sensor == WHITE && right_sensor == WHITE)  {
          UP();
        }                 
     }
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
   if (type == WStype_TEXT){
      String txt = "";
      for(int i = 0; i < length; i++)  {
        txt = txt + ((char)payload[i]);
      }

      command = txt;

      if (command == "cmd1_true"){
          display.clear();
          drawCircleSetup();
          display.display();
      }

      if (command == "cmd1_false"){
          display.clear();
          drawCircleSetup();
          display.display();
      }
      if (command == "cmd2_true"){
          linefollower = true;
      }
      if (command == "cmd2_false"){
          linefollower = false;
      }
         
      if (command == "up"){
        UP();        
      } else if (command == "down"){
        DOWN();
      } else if (command == "left"){
        LEFT();
      } else if (command == "right"){
        RIGHT();
      } else if (command == "end"){
        STOP();
      } else if (command == "right_up"){
        RIGHT_UP();
      } else if (command == "left_up"){
        LEFT_UP();
      } else if (command == "left_down"){
        LEFT_DOWN();
      } else if (command == "right_down"){
        RIGHT_DOWN();
      }
   }
}
 
void UP() {
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
}

 
void LEFT_UP() {  
    LEFT();
    delay(1);
    UP();
    delay(1);
}

 
void RIGHT_UP() {
    RIGHT();
    delay(1);
    UP();
    delay(1);
}

void DOWN() {
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH); 
}

void LEFT_DOWN() {
    LEFT();
    delay(1);
    DOWN();
    delay(1);
}

void RIGHT_DOWN() {
    RIGHT();
    delay(1);
    DOWN();
    delay(1);
}

void LEFT (void)
{
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    digitalWrite(in3,HIGH);  
    digitalWrite(in4,HIGH);

}

void RIGHT (void)
{
    digitalWrite(in1,HIGH);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,LOW);
    digitalWrite(in4,LOW);
}

void STOP (void)
{
    digitalWrite(in1,HIGH);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,HIGH); 
    digitalWrite(in4,HIGH); 
}
