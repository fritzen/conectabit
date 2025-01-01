#include <Servo.h>
#include <SoftwareSerial.h>
#define STICK_NONE  0
#define STICK_UP  1
#define STICK_UPRIGHT  2
#define STICK_RIGHT  3
#define STICK_DOWNRIGHT  4
#define STICK_DOWN 5
#define STICK_DOWNLEFT 6
#define STICK_LEFT 7
#define STICK_UPLEFT 8

#define SOP '<'
#define EOP '>'

#define PIN_H_BRIDGE_1 7
#define PIN_H_BRIDGE_2 9

#define RC_PIN_1 3   // Pin Connected to CH1 of Transmitter;
#define RC_PIN_2 4   // Pin Connected to CH2 of Transmitter;


#define THRESHOLD_PEDAL_SENSOR 512 //0 QUANDO DESCONECTADO E 1024 QUANDO CONECTADO


//Valores mínimo, médio e máximo lidos pelos sensores (direcional)
//CH1
#define LEFT_RIGHT_MIN 1190
#define LEFT_RIGHT_MID 1440
#define LEFT_RIGHT_MAX 1721
#define LEFT_RIGHT_DEADZONE 150


//Valores mínimo, médio e máximo lidos pelos sensores (acelerador)
//CH2
#define UP_DOWN_MIN 770 
#define UP_DOWN_MID 1430
#define UP_DOWN_MAX 1964
#define UP_DOWN_DEADZONE 150



#define DEFAULT_FORWARD_SPEED  180
#define DEFAULT_BACKWARD_SPEED  50

#define MANUAL_BACKWARD  0
#define MANUAL_FORWARD  1
#define MANUAL_DISABLED  2

short lastMode = MANUAL_DISABLED;

SoftwareSerial mySerial(5, 6); //0,1

#define DEBUG  true
bool started = false;
bool ended = false;
bool radioFound = true;
String inData = "";


Servo esc;

void setup()
{
  pinMode(RC_PIN_1, INPUT);
  pinMode(RC_PIN_2, INPUT);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  

  pinMode(PIN_H_BRIDGE_1, OUTPUT);
  pinMode(PIN_H_BRIDGE_2, OUTPUT);

  esc.attach(11);
  esc.write(90);  

  Serial.begin(9600);

  Serial.println("*** START ****");

  mySerial.begin(38400);  

  int  ch1 = pulseIn(RC_PIN_1, HIGH); 
  int  ch2 = pulseIn(RC_PIN_2, HIGH);

  if (ch1 == 0 && ch2 == 0){
    radioFound = false;
    turnRight();
    delay(300);
    turnLeft();
    delay(400);
    turnOff();
  }
    
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
    delay(100);
  } 
}

void loop()
{

  bool hasRCCommand = false;

  int ch1 = 0;  // Receiver Channel 1 PPM value
  int ch2 = 0;  // Receiver Channel 2 PPM value


  if (radioFound) {
    //RC MODE
    // Read in the length of the signal in microseconds
    ch1 = pulseIn(RC_PIN_1, HIGH);
    ch2 = pulseIn(RC_PIN_2, HIGH);
  }
  
  debug("ch1",ch1);
  debug("ch2",ch2); // up-down

  short front = analogRead(A0);
  short back = analogRead(A1);

  debug("front", front);
  debug("back", back);
   
    /*** 
     * LEFT / RIGHT 
     */
     
    if (ch1 > 0 &&ch1 < LEFT_RIGHT_MID - LEFT_RIGHT_DEADZONE) {
      turnLeft();
      hasRCCommand = true;
    } else if (ch1 > LEFT_RIGHT_MID + LEFT_RIGHT_DEADZONE) {
      turnRight();
      hasRCCommand = true;
    } else {
      turnOff();
    }
    
    //UP
    if (ch2 > 0 && ch2 < UP_DOWN_MID - UP_DOWN_DEADZONE){
      
        int servo_mov = map(ch2, UP_DOWN_MIN, UP_DOWN_MID, 90, 180);
        servo_mov = (180 - servo_mov) + 70;
       
        esc.write(servo_mov);
        debug("servo_mov_up",servo_mov);
        hasRCCommand = true;
        
    } else if (ch2 > 0 && ch2 > UP_DOWN_MID + UP_DOWN_DEADZONE){ //Tratar DOWN
        int servo_mov = map(ch2, UP_DOWN_MID, UP_DOWN_MAX, 0, 90);
        servo_mov = 120 - servo_mov;
        esc.write(servo_mov);
        debug("servo_mov_down",servo_mov);
        hasRCCommand = true;
    } else {

          bool manualControl = false;
          /** 
           * throttle (UP & DOWN) 
           */   
          //Controle manual do Lucas, somente se sensor no DEADZONE (indatividade)
         // if (ch2 >= UP_DOWN_MID - UP_DOWN_DEADZONE && ch2 <= UP_DOWN_MID + UP_DOWN_DEADZONE)
          {
              if (front > THRESHOLD_PEDAL_SENSOR) {
  
                if (lastMode != MANUAL_FORWARD) {
                  for (int pos = 0; pos <= DEFAULT_FORWARD_SPEED; pos += 1) {
                      esc.write(pos);
                      delay(1);     
                  }
                  debug("DEFAULT_FORWARD_SPEED",DEFAULT_FORWARD_SPEED);
                }
                manualControl = true;
                lastMode = MANUAL_FORWARD;
              }
              
              if (back > THRESHOLD_PEDAL_SENSOR) {
                 if (lastMode != MANUAL_BACKWARD) {
                    for (int pos = 90; pos >= DEFAULT_BACKWARD_SPEED; pos -= 1) { 
                      esc.write(pos);
                      delay(1);   
                    }
                    debug("DEFAULT_BACKWARD_SPEED",DEFAULT_BACKWARD_SPEED);
                 }
                 manualControl = true;
                 lastMode = MANUAL_BACKWARD;
              }
          }

          if (!manualControl) {
                //STOP motor
                esc.write(90);
                lastMode = MANUAL_DISABLED;
            }
    }


  
    
    if (!hasRCCommand) {
          //BLUETOOTH MODE
          debug("hasRCCommand",hasRCCommand);
          if (mySerial.available() > 10) {
            while(mySerial.available() > 0)
              {
                char inChar = mySerial.read();                
                debug("inChar",inChar);
                                
                if(inChar == SOP)
                {
                  inData = "";
                  started = true;
                  ended = false;
                }
                else if(inChar == EOP)
                {
                  if (inData.length() == 4) {
                    ended = true;
                    break;
                  }
                }
                else
                {
                    inData = inData + inChar;
                }
                delay(1);
              }
          }
          

          if(started && ended) {
              
                started = false;
                ended = false;
          
                short stick = inData.substring(0, 1).toInt(); //0 skip, 1 char
                short dist = inData.substring(2).toInt(); //1_xxx
            
                inData = "";
          
                short escValue = 90; // 90 = 0FF esc; 180 = RIGHT max e 0 = LEFT max
              
                //UP
                if (stick == STICK_UP || stick == STICK_UPRIGHT || stick == STICK_UPLEFT ) {
                    escValue = map(dist, 10, 99, 90, 180);
                    esc.write(escValue);
                }
            
                //DOWN
                if (stick == STICK_DOWN || stick == STICK_DOWNRIGHT || stick == STICK_DOWNLEFT ) {
                    escValue = map(dist, 10, 99, 0, 90);
                    escValue = 90 - escValue;
                    esc.write(escValue);
                }        
            
                //LEFT / RIGHT
                if (stick == STICK_UP || stick == STICK_DOWN  || stick == STICK_NONE) {
                  turnOff();
                } else if (stick == STICK_RIGHT || stick == STICK_UPRIGHT || stick == STICK_DOWNRIGHT ) {
                  turnRight();
                } else {
                  turnLeft();
                }

                //Serial.print("esc=");
                //Serial.print(escValue);
                //Serial.print("stick=");
                //Serial.print(stick);
                //Serial.print("; dist=");
                //Serial.println(dist);
         }
      }

  debugln();
}



  void turnOff()
  {
    digitalWrite(PIN_H_BRIDGE_1, LOW);
    digitalWrite(PIN_H_BRIDGE_2, LOW);
    delay(10);
  }
  
  void turnLeft()
  {
    digitalWrite(PIN_H_BRIDGE_1, HIGH);
    digitalWrite(PIN_H_BRIDGE_2, LOW);
    delay(10);
    //turnOff();
    delay(1);
    debug("turnLeft","true");
  }
  void turnRight()
  {
    digitalWrite(PIN_H_BRIDGE_1, LOW);
    digitalWrite(PIN_H_BRIDGE_2, HIGH);
    delay(10);
    ////turnOff();
    delay(1);
    debug("turnRight","true");
  }
