#include <EEPROM.h>
#include <Servo.h>
#include "TimerFreeTone.h"
#include "IRremote.h" //https://github.com/z3t0/Arduino-IRremote

#define IR_PROJ_ON 0
#define IR_PROJ_OFF 1
#define IR_PROJ_LEFT 2
#define IR_PROJ_OK 3


const byte numIRCodes=6;

const byte irCodes[numIRCodes] [77] PROGMEM = {
  {87, 92, 6, 33, 7, 13, 6, 13, 7, 13, 7, 33, 6, 13, 7, 13, 7, 13, 7, 32, 7, 13, 7, 13, 7, 33, 6, 14, 6, 14, 6, 13, 6, 14, 6, 93, 6, 13, 6, 14, 6, 14, 6, 13, 6, 34, 6, 13, 6, 14, 6, 34, 6, 33, 6, 14, 6, 14, 6, 33, 6, 14, 6, 34, 6, 33, 6, 14, 6, 14, 6, 33, 6, 34, 6, 13, 7}, //rawProjOn
  {87, 92, 6, 33, 7, 13, 6, 13, 7, 13, 7, 33, 6, 13, 7, 13, 7, 13, 7, 32, 7, 13, 7, 13, 7, 33, 6, 14, 6, 13, 7, 13, 6, 14, 6, 93, 6, 13, 6, 14, 6, 14, 6, 13, 6, 14, 6, 14, 6, 13, 6, 34, 6, 33, 6, 14, 6, 14, 6, 33, 6, 34, 6, 33, 7, 33, 6, 14, 6, 14, 6, 33, 6, 34, 6, 13, 7}, //rawProjOff
  {88, 91, 7, 32, 8, 11, 8, 12, 8, 12, 8, 32, 7, 12, 8, 12, 8, 12, 8, 32, 7, 12, 8, 12, 8, 32, 7, 12, 8, 12, 8, 12, 8, 12, 7, 92, 7, 12, 8, 12, 7, 12, 8, 12, 8, 32, 7, 12, 8, 32, 7, 12, 8, 12, 8, 32, 7, 33, 7, 12, 8, 12, 7, 33, 7, 12, 8, 32, 7, 32, 8, 12, 8, 12, 7, 32, 8}, //rawProjEsquerda
  {87, 91, 7, 32, 8, 12, 7, 12, 8, 12, 8, 32, 8, 11, 8, 12, 8, 12, 8, 32, 7, 12, 8, 12, 8, 32, 7, 12, 8, 12, 8, 12, 8, 12, 7, 92, 7, 12, 8, 12, 8, 11, 8, 12, 8, 11, 8, 12, 8, 12, 8, 32, 7, 13, 7, 32, 8, 32, 7, 12, 8, 32, 8, 32, 7, 32, 8, 12, 7, 33, 7, 12, 8, 12, 7, 33, 7}, //rawProjOk
  {""}
};  


#define TONE_PIN 8
#define PC_RELAY_PIN 11
#define SERVO1_PIN 9
#define SERVO2_PIN 10
#define RECV_PIN 2


IRsend irsend; //PORTA D3 (PWM)
IRrecv irrecv(RECV_PIN);
decode_results results;


#define ARRAY_SIZE 77
#define KHZ 38

// **PORTA DO ARMÁRIO (VISÃO FRONTAL)
// ---------------------------------------
// |                  |                  |
// |                * | *                |
// | servo2 (left)  * | * servo1 (right) |
// |                * | *                |
// ---------------------------------------

#define DOOR_LEFT_CLOSED 180 // MAIOR FECHA MAIS
#define DOOR_LEFT_OPENED 80  // MENOR ABRE MAIS
#define DOOR_RIGHT_CLOSED 45  // MENOR FECHA MAIS
#define DOOR_RIGHT_OPENED 130 // MAIOR ABRE MAIS

#define OFF_PROJECTOR_SECS 70 //Aguardar 120s
#define DOOR_SPEED 80

int offProjectorSecs = 0;


Servo myservo1;
Servo myservo2;

byte pos1 = 50; // variable to store the servo position
byte pos2 = 50; // variable to store the servo position

bool cinema = false;


void setup()
{

  Serial.begin(9600);
  Serial.println(F("*** INICIANDO CINEMA ***"));

  
  pinMode(PC_RELAY_PIN, OUTPUT);
  digitalWrite(PC_RELAY_PIN, LOW);

  PlayMusic(TONE_PIN);

  pos1 = EEPROM.read(1);
  pos2 = EEPROM.read(2);
  
  irrecv.enableIRIn(); // Start the receiver

  if (pos1 == 255 && pos2 == 255)
  {

    Serial.println(F("*** INICIANDO VALORES SERVOS DEFAULT ***"));
    
    pos1 = DOOR_RIGHT_OPENED;
    pos2 = DOOR_LEFT_OPENED;

    EEPROM.write(1, pos1);
    EEPROM.write(2, pos2);
  }

  myservo1.detach();
  myservo2.detach();

  desligarProjetor();
  
}
void sendIR(const byte buf[], int attempts)
{

  unsigned int code[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  for (byte i = 0; i < ARRAY_SIZE; i++) {
    code[i] = (buf[i] * 5) * 10;
    //Serial.print(code[i]); Serial.print(F(", "));
  }

  for (int x = 0; x < attempts; x++)
  {
    irsend.sendRaw(code, ARRAY_SIZE, KHZ);
    Serial.print(F("SEND IR "));
    Serial.print(x);
    Serial.println(F(""));
    delay(50);
  }

  delay(150);
  irrecv.enableIRIn();
  delay(100);
}

void setPCPowerRelay(int onOff)
{
  digitalWrite(PC_RELAY_PIN, onOff);
}
void ligarProjetor()
{

  Serial.print(F("LIGANDO PROJETOR"));

  byte rawProjOn[ARRAY_SIZE];  
  memcpy_P(&rawProjOn,&irCodes[IR_PROJ_ON],sizeof rawProjOn);

  sendIR(rawProjOn, 50);

  Serial.println(F(" - OK"));
}

void desligarProjetor()
{
  Serial.print(F("DESLIGANDO PROJETOR"));
  
  byte rawProjOff[ARRAY_SIZE];  
  memcpy_P(&rawProjOff,&irCodes[IR_PROJ_OFF],sizeof rawProjOff);

  byte rawProjEsquerda[ARRAY_SIZE];  
  memcpy_P(&rawProjEsquerda,&irCodes[IR_PROJ_LEFT],sizeof rawProjEsquerda);

  byte rawProjOk[ARRAY_SIZE];  
  memcpy_P(&rawProjOk,&irCodes[IR_PROJ_OK],sizeof rawProjOk);

  sendIR(rawProjOff, 3);
  delay(500);
  sendIR(rawProjEsquerda, 3);
  delay(500);
  sendIR(rawProjOk, 5);
  delay(500);

  Serial.println(F(" - OK"));
}


void openDoor()
{
  Serial.print(F("ABRINDO PORTA"));
  
  pos1 = EEPROM.read(1);
  pos2 = EEPROM.read(2);

  //Serial.print(F("POS1: ")); Serial.println(pos1); Serial.print(F("POS2:")); Serial.println(pos2); //DEBUG
  
  myservo1.attach(SERVO1_PIN);
  for (; pos1 <= DOOR_RIGHT_OPENED; pos1 += 1)
  {
    myservo1.write(pos1);
    delay(DOOR_SPEED);
  }
  myservo1.detach();

  delay(500);

  myservo2.attach(SERVO2_PIN);
  for (; pos2 > DOOR_LEFT_OPENED; pos2 -= 1)
  {
    myservo2.write(pos2);
    delay(DOOR_SPEED);
  }
  myservo2.detach();

  Serial.println(F(" - OK"));

  EEPROM.write(1, pos1);
  EEPROM.write(2, pos2);
  
  //Serial.print(F("POS1: ")); Serial.println(pos1); Serial.print(F("POS2:")); Serial.println(pos2); //DEBUG
}


void closeDoor()
{
  Serial.print(F("FECHANDO PORTA"));
  pos1 = EEPROM.read(1);
  pos2 = EEPROM.read(2);

  //Serial.print(F("POS1: ")); Serial.println(pos1); Serial.print(F("POS2:")); Serial.println(pos2); //DEBUG

  myservo1.attach(SERVO1_PIN);
  for (; pos1 > DOOR_RIGHT_CLOSED; pos1 -= 1)
  {
    myservo1.write(pos1);
    delay(DOOR_SPEED);
  }
  myservo1.detach();

  delay(500);

  myservo2.attach(SERVO2_PIN);
  for (; pos2 <= DOOR_LEFT_CLOSED; pos2 += 1)
  {
    myservo2.write(pos2);
    delay(DOOR_SPEED);
  }

  myservo2.detach();

  Serial.println(F(" - OK"));

  EEPROM.write(1, pos1);
  EEPROM.write(2, pos2);

  //Serial.print(F("POS1: ")); Serial.println(pos1); Serial.print(F("POS2:")); Serial.println(pos2); //DEBUG

}

void turnCinemaOn()
{
  Serial.println(F("LIGANDO CINEMA"));
  setPCPowerRelay(HIGH);
  TimerFreeTone(TONE_PIN, 250, 250);
  ligarProjetor();
  TimerFreeTone(TONE_PIN, 250, 250);
  openDoor();
  TimerFreeTone(TONE_PIN, 250, 250);
  TimerFreeTone(TONE_PIN, 250, 250);  
  cinema = true;  
  Serial.println(F("CINEMA LIGADO"));
}

void turnCinemaOff()
{
  Serial.println(F("DESLIGANDO CINEMA"));
  desligarProjetor();
  desligarProjetor();
  setPCPowerRelay(LOW);
  TimerFreeTone(TONE_PIN, 250, 250);  
  desligarProjetor();
  closeDoor();
  TimerFreeTone(TONE_PIN, 250, 250);
  TimerFreeTone(TONE_PIN, 250, 250);
  offProjectorSecs = OFF_PROJECTOR_SECS;
  desligarProjetor();
  cinema = false;  
  Serial.println(F("CINEMA DESLIGADO"));
}

int contadorPower = 0;
void loop()
{


  bool hasIrSignal = false;

  if (irrecv.decode(&results)) {
      Serial.print(results.value, HEX);
      Serial.print(" - ");
      Serial.println(contadorPower);
      
      if (results.value == 0xCA31DA45) {
        hasIrSignal = true;
      }
           
      irrecv.resume(); // Receive the next value
  }


  if (hasIrSignal) {
    contadorPower++;
    TimerFreeTone(TONE_PIN, 200, 100);
    if (offProjectorSecs > 0) { //aguardar segundos
        TimerFreeTone(TONE_PIN, 200, 300);
        TimerFreeTone(TONE_PIN, 400, 700);
        Serial.print(F("AGUARDANDO PROJETOR ESFRIAR - "));
        Serial.print(offProjectorSecs);
        Serial.println(F(" SEGUNDOS RESTANTES"));
        
    } else {
      if (cinema == false) {
          turnCinemaOn();
      }   
    }

    

  } else {
    contadorPower = 0;  
  }

  if (contadorPower >= 5) {
     if (cinema) { //se desligar projetor e cinema ligado, entao desligar cinema
         turnCinemaOff();
     }
  }

  delay(500);

  if (offProjectorSecs > 0){
    offProjectorSecs--;
  }

  
}
