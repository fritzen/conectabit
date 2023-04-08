#include <Servo.h>
#include <NeoSWSerial.h>
//#include <SoftwareSerial.h>
#define STICK_NONE  0
#define STICK_UP  1
#define STICK_UPRIGHT  2
#define STICK_RIGHT  3
#define STICK_DOWNRIGHT  4
#define STICK_DOWN 5
#define STICK_DOWNLEFT 6
#define STICK_LEFT 7
#define STICK_UPLEFT 8

#define PIN_H_BRIDGE_1 7
#define PIN_H_BRIDGE_2 9


#define THRESHOLD_PEDAL_SENSOR 512 //0 QUANDO DESCONECTADO E 1024 QUANDO CONECTADO


#define DEFAULT_FORWARD_SPEED  180
#define DEFAULT_BACKWARD_SPEED  50

#define MANUAL_BACKWARD  0
#define MANUAL_FORWARD  1
#define MANUAL_DISABLED  2
short lastMode = MANUAL_DISABLED;


NeoSWSerial mySerial(5, 6);

//SEGURANCA; PARAR CARRINHO CASO PERCA COMUNICAÇÃO POR <intervalo> milisegundos
int securityOff = 0;
#define MAX_INTERVAL  100

#define DEBUG  false

bool started = false;
bool ended = false;
String inData = "";

bool allowKids = false;

Servo esc;

void setup()
{

  Serial.begin(9600);

  mySerial.attachInterrupt( handleRxChar );
  mySerial.begin( 9600 );
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  
  pinMode(PIN_H_BRIDGE_1, OUTPUT);
  pinMode(PIN_H_BRIDGE_2, OUTPUT);

  esc.attach(11);
  delay(10); 
}


void debug(String key, bool value) {
  debug(key, String(value));
}

void debug(String key, int value) {
  debug(key, String(value));
}

void debug(String key, String value) {
  if (DEBUG) {
    Serial.print(key + "=" + value + "; ");
  } 
}

void debugln() {
  if (DEBUG) {
    Serial.println("");
  } 
}

bool manualControl = false;
void doKidsControl() {
    /** 
  * throttle criança (UP & DOWN) 
  */
  
  short frontKids = analogRead(A0);
  short backKids = analogRead(A1);
  manualControl = false;
  
  debug("FRONT", frontKids);
  debug("BACK ", backKids);
  
  if (frontKids > THRESHOLD_PEDAL_SENSOR) {  
    if (lastMode != MANUAL_FORWARD) {
      for (int pos = 0; pos <= DEFAULT_FORWARD_SPEED; pos += 1) {
          esc.write(pos);
          delay(1);     
      }
    }
    manualControl = true;
    lastMode = MANUAL_FORWARD;
  }
  
  if (backKids > THRESHOLD_PEDAL_SENSOR) {
    if (lastMode != MANUAL_BACKWARD) {
      for (int pos = 90; pos >= DEFAULT_BACKWARD_SPEED; pos -= 1) { 
        esc.write(pos);
        delay(1);   
      }
    }
    manualControl = true;
    lastMode = MANUAL_BACKWARD;
  }


  if (!manualControl) {
      //STOP motor
      esc.write(90);
      lastMode = MANUAL_DISABLED;
  }

}

bool expectedDirection = false;
bool expectedStrength = false;
short stick = STICK_NONE;
short dist = 0;
  
void doBluetoothAction() {


      inData = "";
      started = false;
      ended = false;
      
      short escValue = 90; // 90 = 0FF esc; 180 = RIGHT max e 0 = LEFT max

      if ( stick == STICK_NONE ) {
         esc.write(90);
         turnOff();
      } else {    
          //UP
          if (stick == STICK_UP || stick == STICK_UPRIGHT || stick == STICK_UPLEFT ) {
              escValue = map(dist, 0, 9, 90, 180);
              Serial.print("UP=");
              Serial.println(escValue);
              esc.write(escValue);
              securityOff = 0;
          }  
          //DOWN
          if (stick == STICK_DOWN || stick == STICK_DOWNRIGHT || stick == STICK_DOWNLEFT ) {
              escValue = map(dist, 0, 9, 0, 90);
              escValue = 90 - escValue;
              Serial.print("DOWN=");
              Serial.println(escValue);
              esc.write(escValue);
              securityOff = 0;
          }        
      
          //LEFT / RIGHT
          if (stick == STICK_UP || stick == STICK_DOWN) {
            turnOff();
          } else if (stick == STICK_RIGHT || stick == STICK_UPRIGHT || stick == STICK_DOWNRIGHT ) {
            turnRight();
          } else {
            turnLeft();
          }
      }

}



    static void handleRxChar( uint8_t c )
    {
      //Serial.println((char)c);
      //D1S9
      
      securityOff = 0;
      char inChar = c;                
      
      if (expectedDirection) {
        stick =  (int)inChar - '0'; //
        //Serial.println(stick);
        expectedDirection = false;
      }
      if (expectedStrength) {
        dist =  (int)inChar - '0'; //
       // Serial.println(dist);
        expectedStrength = false;
        ended = true;
      }
      
      if(inChar == 'D')
      {
        //Serial.print("DIRECTION=");
        expectedDirection = true;
        expectedStrength = false;
        started = true;
      }
      if(inChar == 'S')
      {
        //Serial.print("STRENT=");
        expectedDirection = false;
        expectedStrength = true;          
      }        
    }
    
void loop()
{     

  if (securityOff > MAX_INTERVAL) {
      Serial.println("SECURITY OFF");
      securityOff = 0;
      turnOff();
      if (!manualControl) {
          esc.write(90); //turn off  
      }      
      allowKids = true;  
  }
    
  if(started && ended) {
    started =false;
    ended = false;
    doBluetoothAction();
    securityOff = 0; //novo comando em menos de MAX_INTERVAL ms
    allowKids = false;
  } else if (allowKids) {
    doKidsControl();
  }

  delay(1);
  securityOff++;


}

void turnOff()
{
  digitalWrite(PIN_H_BRIDGE_1, LOW);
  digitalWrite(PIN_H_BRIDGE_2, LOW);
}

void turnLeft()
{
    digitalWrite(PIN_H_BRIDGE_1, HIGH);
    digitalWrite(PIN_H_BRIDGE_2, LOW);
    delay(1);
}
void turnRight()
{
  digitalWrite(PIN_H_BRIDGE_1, LOW);
  digitalWrite(PIN_H_BRIDGE_2, HIGH);
  delay(1);
}
