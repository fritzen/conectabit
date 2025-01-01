
// Vermelho: GND
// SDA : Amarelo
// SCL : Branco
// Verde: VDD
// Preto: Detect

/*
For the original Nunchuk, the following is the color and pin-layout of the connector:

Green → SDA
nc
Red → 3v3
White → GND
nc
Yellow → SCL


For the Nunchuk clone I got, the following is the color and pin-layout of the connector:

Yellow → SDA
Black → nc (Detect)
Green → 3v3
Red → GND
nc
White → SCL


A4 - SDA 
A5 - SCL

*/

#include <Wire.h>
#include <Servo.h>
#include "TimerFreeTone.h"
#include "nunchuck_funcs.h"      //Controle Wii NunChuck

//Contantes
#define MAX_SERVO 180
#define MIN_SERVO 0
#define PIN_ELETROIMA 2

#define PIN_SERVO 3

#define PIN_1_MOTOR_ELETROIMA 4
#define PIN_2_MOTOR_ELETROIMA 5

#define PIN_LED_1 6
#define PIN_LED_2 7


#define DEFAULT_TONE_PIN 8

// Comunicação via protocolo I2C COM Nunchuck: A4 (SDA), A5 (SCL)


 
// Valores de cada um dos servos
int countServo = 90;
bool ledSway = false;

Servo servoBase;

bool eletroImaState = true;


void song() {
  int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
  int duration[] = { 250, 125, 125, 250, 250, 250, 250, 250 };
  for (int thisNote = 0; thisNote < 8; thisNote++) { // Loop through the notes in the array.
    TimerFreeTone(DEFAULT_TONE_PIN, melody[thisNote], duration[thisNote]); // Play thisNote for duration.
    delay(50); // Short delay between notes.
  }
}

void setup() {

  Serial.begin(115200);
  Serial.println("*** Guindaste Articulado ***");

  nunchuck_setpowerpins();
  nunchuck_init();

  
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_ELETROIMA, OUTPUT);
  pinMode(PIN_1_MOTOR_ELETROIMA, OUTPUT);
  pinMode(PIN_2_MOTOR_ELETROIMA, OUTPUT);


  
  
  servoBase.attach(PIN_SERVO);
  servoBase.write(countServo);
  song();

}


void MotorSubirIma() {
  digitalWrite(PIN_1_MOTOR_ELETROIMA, LOW);
  digitalWrite(PIN_2_MOTOR_ELETROIMA, HIGH);
}

void MotorDescerIma() {
  digitalWrite(PIN_1_MOTOR_ELETROIMA, HIGH);
  digitalWrite(PIN_2_MOTOR_ELETROIMA, LOW);
}

void MotorPararIma() {
  digitalWrite(PIN_1_MOTOR_ELETROIMA, LOW);
  digitalWrite(PIN_2_MOTOR_ELETROIMA, LOW);
}

// Esta funcao é responsável por retornar o incremento de acordo com o valor passado como parâmetro
int ObterIncremento(int value) {
  
    int var = -1;

    var = map(value, 0, 255, 1, 7); //map(value, fromLow, fromHigh, toLow, toHigh);

    switch (var) {
    case 1:
      return 2; //dobro
      break;
    case 2: case 3:
      return 1; 
      break;
    case 4:  //meio
      return 0;
      break;
    case 5: case 6:
      return -1; 
      break;
    case 7:
      return -2; //dobro
      break;      
    default: 
      return 0;
    break;
  }
}

bool stateCanChange = true;

unsigned long millisTarefa1 = millis();


void loop() {

    nunchuck_get_data();      //coleta dados do nunchuck

    byte accx, accy, zbut, cbut, joyx, joyy;


    joyx = nunchuck_joyx();   //joystick X (não utilizado)
    joyy = nunchuck_joyy();   //joystick Y (controle vertical do braço)

    accx  = nunchuck_accelx(); //acelerômetro X (controle da base do braço)
    accy  = nunchuck_accely(); //acelerômetro Y (controle frente/trás do braço)
    zbut = nunchuck_zbutton(); //botão Z (controle da garra), pressionado garra fechada, solto garra aberta
    cbut = nunchuck_cbutton(); //botão C (quando pressionado, congela o braço)
   

    
   
    if (joyx < 20 || (cbut && accx < 80))  { // esquerda
        countServo--;
        if (countServo < MIN_SERVO) {
          countServo = MIN_SERVO;
        }
         if (!eletroImaState) {
          delay(50);
         }
    } else if (joyx > 220 || (cbut && accx > 150)) { 
      countServo++;
      if (countServo > MAX_SERVO) {
          countServo = MAX_SERVO;
        }
        if (!eletroImaState) {
          delay(50);
         }
    }
//x < 80; >150
//y > 150 frente// <110 traz
    servoBase.write(countServo);    

    if (joyy < 20 || (cbut && accy < 110))  { // esquerda
      MotorSubirIma();
    } else if (joyy > 220 || (cbut && accy > 150)) { 
      MotorDescerIma();
    } else {
      MotorPararIma();
    }
  
  nunchuck_print_data();

  if (zbut && stateCanChange)
  {     
    eletroImaState = !eletroImaState;
    stateCanChange = false;
  }

  
  if (!zbut){
    stateCanChange = true;
  }
    

  tarefaPisca();
  switchState();


}


void tarefaPisca(){



  // Verifica se já passou 200 milisegundos
  if((millis() - millisTarefa1) < 200){  //0.200ms

      switchState();
    
  } else { //201..400ms

  }
  // Verifica se já passou 400 milisegundos
  if((millis() - millisTarefa1) > 400){
    millisTarefa1 = millis();
  }
}

void switchState() {
      if (eletroImaState) {
        digitalWrite(PIN_ELETROIMA, HIGH);
        ledSway = !ledSway;
        if (ledSway) {
            digitalWrite(PIN_LED_1, LOW);
            digitalWrite(PIN_LED_2, HIGH);
            TimerFreeTone(DEFAULT_TONE_PIN, 200, 100);
        } else {
            digitalWrite(PIN_LED_1, HIGH);
            digitalWrite(PIN_LED_2, LOW);
            TimerFreeTone(DEFAULT_TONE_PIN, 400, 100);
        }

    } else  {
        digitalWrite(PIN_ELETROIMA, LOW);
        digitalWrite(PIN_LED_1, LOW);
        digitalWrite(PIN_LED_2, LOW);
    }
}