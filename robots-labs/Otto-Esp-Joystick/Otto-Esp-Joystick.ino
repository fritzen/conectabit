#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <WebSocketsServer.h> //Websockets Lib, By Markus https://github.com/Links2004/arduinoWebSockets

//---------------------------------------------------------
//-- First step: Make sure the pins for servos are in the right position
/*
         --------------- 
        |     O   O     |
        |---------------|
YR 3==> |               | <== YL 2
         --------------- 
            ||     ||
RR 5==>   -----   ------  <== RL 4
         |-----   ------|
*/

///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
//-- Movement parameters
int T=1000;              //Initial duration of movement
int moveId=0;            //Number of movement
int moveSize=15;         //Asociated with the height of some movements
int piezo=A0;

#define PIN_YR 13 //servo[3] !!Choose your ESP32 pin!! 
#define PIN_RR 12 //servo[5] !!Choose your ESP32 pin!!
#define PIN_YL 14 //servo[2] !!Choose your ESP32 pin!! 
#define PIN_RL 27 //servo[4] !!Choose your ESP32 pin!!


#include "ESP32Servo.h"
#include "OttoESP.h"
Otto Otto; 

#include "webpage.h"
#include "css.h"
#include "js.h"
#include "html.h"

WebSocketsServer webSocket = WebSocketsServer(81);
WebServer server(80);

uint8_t pin_led = 16;
char* ssid = "YOUR_SSID";
char* password = "YOUR_PASSWORD";

IPAddress local_IP(192,168,0,2);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

int delayTime = 0;

String command = "";

int pos = 0;    // variable to store the servo position

void setup()
{

  Otto.init(PIN_YL,PIN_YR,PIN_RL,PIN_RR,true,piezo,piezo);
  Otto.sing(S_connection); //Otto wake up!
  Otto.home();
  delay(500);

  //WiFi.begin(ssid,password);
  //ATENÇAO: NAO PODE HABILITAR SERIAL POIS GPIO1 (TX) E GPIO3 (RX) SAO UTILIZADOS PARA CONTROLE DO MOTOR
  Serial.begin(9600); 
  STOP();
 
  WiFi.mode(WIFI_AP);  
  

  
  WiFi.softAPConfig(local_IP, gateway, subnet); //? "Ready" : "Failed!"
  WiFi.softAP("ROBOT-NETWORK-2");

  //Serial.println(WiFi.softAPIP());

  server.on("/nipplejs.js",[](){server.send_P(200,"application/javascript", js);}); 
  server.on("/styles.css",[](){server.send_P(200,"text/css", css);});
  server.on("/",[](){server.send_P(200,"text/html", html);});
  server.on("/joy2.html",[](){server.send_P(200,"text/html", webpage);});

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);    
  server.begin();
}
void loop() {  
    //webSocket.loop();
    //server.handleClient();

     Otto.walk(2,500,-1); //change T for lower value to run faster!
                              
             delay(50); 
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
   if (type == WStype_TEXT){
      String txt = "";
      for(int i = 0; i < length; i++)  {
        txt = txt + ((char)payload[i]);
      }

      command = txt;
         
      if (command == "up"){
        UP();        
      } else if (command == "down"){
        DOWN();
      } else if (command == "left" || command == "left_up" || command == "left_down"){
        MOVE_LEFT();
      } else if (command == "right"|| command == "right_up" || command == "right_down"){
        MOVE_RIGHT();
      } else if (command == "end"){
        STOP();
      }
   }
}

 
void UP() {  //Forward 
    Otto.walk(2,1000,1); //2 steps FORWARD
}

void DOWN() { //Backward     
     Otto.walk(2,1000,-1); //2 steps BACKSWARD
}

void MOVE_LEFT ()
{
    Otto.turn(2,1000,-1);//2 steps turning LEFT 
}

void MOVE_RIGHT ()
{
    Otto.turn(2,1000,1);//2 steps turning RIGHT 
}

void STOP (void)
{
    Otto.home();
}

