//#include "IRremote.h"

#include "ir_command_codes.h"
#include "HCSR04.h"

#include <Servo.h>


const int TRIG_PIN = 7; // Pino do trigger do sensor HC-SR04 (conectado ao pino digital 6 do Arduino)
const int ECHO_PIN = 6; // Pino do echo do sensor HC-SR04 (conectado ao pino digital 7 do Arduino)


HCSR04 ultrasonicSensor(TRIG_PIN, ECHO_PIN);


// EN: Analog pin where IR detector is pluged in.
const int IR_PIN = A0;

// EN: Servo pins.
const int LEFT_SERVO_PIN = 4;
const int CENTRAL_SERVO_PIN = 2;
const int RIGHT_SERVO_PIN = 3;



// EN: Servo "zero" angle positions.
const long LEFT_SERVO_ZERO_VALUE = 90;
const long RIGHT_SERVO_ZERO_VALUE = 75;
const long CENTRAL_SERVO_ZERO_VALUE = 80;

// EN: Amplitude of left and right servos.
const long SIDE_SERVOS_FULL_AMPLITUDE = 30;
// EN: Half amplitude of left and right servos. Is used when robot is turning
//     left or right while moving forward or backward.
const long SIDE_SERVOS_HALF_AMPLITUDE = 15;
// EN: Amplitude of central servo.
const long CENTRAL_SERVO_AMPLITUDE = 40;

// EN: Periods for different speeds.
const long STEP_PERIOD_VERY_SLOW = 2000;
const long STEP_PERIOD_SLOW = 1500;
const long STEP_PERIOD_FAST = 1000;
const long STEP_PERIOD_VERY_FAST = 500;

long lastMillis;
long globalPhase;
float angleShiftLeftServo;
float angleShiftRightServo;
float angleShiftCentralServo;
long stepPeriod;
long amplitudeLeftServo;
long amplitudeRightServo;
boolean isAttached;
boolean isStopped;

// EN: IRrecv class performs the decoding.
//IRrecv irrecv(IR_PIN);

Servo LeftServo;
Servo RightServo;
Servo CentralServo;

void attachServos() {
  if (!isAttached) {
    LeftServo.attach(LEFT_SERVO_PIN);
    RightServo.attach(RIGHT_SERVO_PIN);
    CentralServo.attach(CENTRAL_SERVO_PIN);
    isAttached = true;
  }
}

// EN: In some positions servos can make noise and vibrate.
//     To avoid this noise and vibration detach servos when robot is stopped.
void detachServos() {
  if (isAttached) {
    LeftServo.detach();
    RightServo.detach();
    CentralServo.detach();
    isAttached = false;
  }
}

void setup() {
  // EN: Start the IR receiver.
//  irrecv.enableIRIn();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

   Serial.begin(9600);
   Serial.println("BEGIN");

  attachServos();
  isStopped = true;
  lastMillis = millis();

  angleShiftLeftServo = 0;
  angleShiftRightServo = 0;
  angleShiftCentralServo = 0;
  
  stepPeriod = STEP_PERIOD_FAST;
  ultrasonicSensor.begin();
}

// EN: Gets angle for servo.
//     Param amplitude - amplitude of oscillating process,
//     param phaseMillis - current duration of oscillating,
//     param shiftAndle - phase of oscillating process.
int getAngle(long amplitude, long phaseMillis, float shiftAngle) {
  float alpha = 2 * PI * phaseMillis / stepPeriod + shiftAngle;
  float angle = amplitude * sin(alpha);
  return (int)angle;
}

template<typename T,size_t N>
boolean hasCode(T (&commandCodes)[N], long code) {
  for (int i = 0; i < N; i++) {
    if (commandCodes[i] == code) {
      return true;
    }
  }
  return false;
}


int contador = 0;

void loop() {

  
  long millisNow = millis();
  long millisPassed = millisNow - lastMillis;
  if (isStopped) {
    // EN: We should wait for half a second. After that we think that servos are in zero
    //     position and we can detach them.
    if (millisPassed >= 500) {
      lastMillis = 0;
      detachServos();
    }
    globalPhase = 0;
  } else {
    lastMillis = millisNow;  
    globalPhase += millisPassed;
    globalPhase = globalPhase % stepPeriod;
  }

  long distance = ultrasonicSensor.getDistance();


  // Verifica se o tempo de retorno é válido (diferente de 0)
  // if (duration > 100) {
  //   // Calcula a distância em milímetros
  //   float distance_mm = duration * 0.034 / 2 * 10;

  //   // Verifica se a distância é menor que 100 mm (10 cm)
  //   if (distance_mm < 100) {
  //     // Coloque aqui o código que deseja executar caso a distância seja menor que 100 mm
  //     //Serial.println("Distancia menor que 100mm!");
  //   }

  //   // Exibe a distância no monitor serial em milímetros
  //   Serial.print("Distancia: ");
  //   Serial.print(distance_mm);
  //   Serial.println(" mm");
  // } 



  if (distance > 500 && contador == 0) {
      attachServos(); //andar pra frente
      isStopped = false;
      angleShiftLeftServo = 0;
      angleShiftRightServo = 0;
      angleShiftCentralServo = -PI/2;
      stepPeriod = STEP_PERIOD_FAST;
      amplitudeLeftServo = SIDE_SERVOS_FULL_AMPLITUDE;
      amplitudeRightServo = SIDE_SERVOS_FULL_AMPLITUDE;
      Serial.print(contador);+
      Serial.print("-AVANCAR-");
  } else  if (distance < 500) {
      contador = 500;
  }

  if (contador > 250) {

      Serial.print(contador);
      Serial.print("-RECUAR-");
      attachServos(); //recuar
      isStopped = false;
      angleShiftLeftServo = 0;
      angleShiftRightServo = 0;
      angleShiftCentralServo = PI/2;
      stepPeriod = STEP_PERIOD_VERY_FAST;  
      amplitudeLeftServo = SIDE_SERVOS_FULL_AMPLITUDE;
      amplitudeRightServo = SIDE_SERVOS_FULL_AMPLITUDE;
  } else if (contador > 0) {
      Serial.print(contador);
      Serial.print("-VIRAR-");
      attachServos(); //virar
      isStopped = false;
      angleShiftLeftServo = 0;
      angleShiftRightServo = PI;
      angleShiftCentralServo = -PI/2;
      stepPeriod = STEP_PERIOD_FAST;
      amplitudeLeftServo = SIDE_SERVOS_FULL_AMPLITUDE;
      amplitudeRightServo = SIDE_SERVOS_FULL_AMPLITUDE;

  }

    contador--;

    if (contador <=0) {
      contador = 0;
    }


Serial.println(distance);


// EN: Declaration of the structure that is used for received and decoded IR commands.
//  decode_results results;
  
  // EN: We can handle IR command if it is received and decoded successfully.
  //if (irrecv.decode(&results)) {
    
    // if (hasCode(IR_COMMAND_FORWARD_CODES, results.value) ||
    //     hasCode(IR_COMMAND_FORWARD_LEFT_CODES, results.value) ||
    //     hasCode(IR_COMMAND_FORWARD_RIGHT_CODES, results.value)) {
    
      // attachServos();
      // isStopped = false;
      // angleShiftLeftServo = 0;
      // angleShiftRightServo = 0;
      // angleShiftCentralServo = -PI/2;
        
      // amplitudeLeftServo = SIDE_SERVOS_FULL_AMPLITUDE;
      // amplitudeRightServo = SIDE_SERVOS_FULL_AMPLITUDE;
      // if (hasCode(IR_COMMAND_FORWARD_LEFT_CODES, results.value)) {
      //   amplitudeLeftServo = SIDE_SERVOS_HALF_AMPLITUDE; 
      // } else if (hasCode(IR_COMMAND_FORWARD_RIGHT_CODES, results.value)) {
      //   amplitudeRightServo = SIDE_SERVOS_HALF_AMPLITUDE;
      // }
    // } else if(hasCode(IR_COMMAND_BACKWARD_CODES, results.value) ||
    //           hasCode(IR_COMMAND_BACKWARD_LEFT_CODES, results.value) ||
    //           hasCode(IR_COMMAND_BACKWARD_RIGHT_CODES, results.value)) {

    //   attachServos();
    //   isStopped = false;
    //   angleShiftLeftServo = 0;
    //   angleShiftRightServo = 0;
    //   angleShiftCentralServo = -PI/2;

    //   amplitudeLeftServo = SIDE_SERVOS_FULL_AMPLITUDE;
    //   amplitudeRightServo = SIDE_SERVOS_FULL_AMPLITUDE;
    //   if (hasCode(IR_COMMAND_BACKWARD_LEFT_CODES, results.value)) {
    //     amplitudeRightServo = SIDE_SERVOS_HALF_AMPLITUDE;
    //   } else if (hasCode(IR_COMMAND_BACKWARD_RIGHT_CODES, results.value)) {
    //     amplitudeLeftServo = SIDE_SERVOS_HALF_AMPLITUDE;
    //   }
    // } else if (hasCode(IR_COMMAND_TURN_LEFT_CODES, results.value)) {
    //   attachServos();
    //   isStopped = false;
    //   angleShiftLeftServo = 0;
    //   angleShiftRightServo = PI;
    //   angleShiftCentralServo = -PI/2;
    //   amplitudeLeftServo = SIDE_SERVOS_FULL_AMPLITUDE;
    //   amplitudeRightServo = SIDE_SERVOS_FULL_AMPLITUDE;
    // } else if (hasCode(IR_COMMAND_TURN_RIGHT_CODES, results.value)) {
    //   attachServos();
    //   isStopped = false;
    //   angleShiftLeftServo = 0;
    //   angleShiftRightServo = PI;
    //   angleShiftCentralServo = PI/2;
    //   amplitudeLeftServo = SIDE_SERVOS_FULL_AMPLITUDE;
    //   amplitudeRightServo = SIDE_SERVOS_FULL_AMPLITUDE;
    // } else if (hasCode(IR_COMMAND_STOP_CODES, results.value)) {
    //   attachServos();
    //   isStopped = true;
    //   angleShiftLeftServo = 0;
    //   angleShiftRightServo = 0;
    //   angleShiftCentralServo = 0;
    //   amplitudeLeftServo = SIDE_SERVOS_FULL_AMPLITUDE;
    //   amplitudeRightServo = SIDE_SERVOS_FULL_AMPLITUDE;
    // } else if (hasCode(IR_COMMAND_VERY_SLOW_CODES, results.value)) {
    //   // EN: globalPhase correction to save servo positions when changing period.
    //   // RU: Корректировка globalPhase чтобы сохранить положение серв при смене периода колебаний ног.
    //   globalPhase = globalPhase * STEP_PERIOD_VERY_SLOW / stepPeriod;
       
    // } else if (hasCode(IR_COMMAND_SLOW_CODES, results.value)) {
    //   globalPhase = globalPhase * STEP_PERIOD_SLOW / stepPeriod;
    //   stepPeriod = STEP_PERIOD_SLOW;
    // } else if (hasCode(IR_COMMAND_FAST_CODES, results.value)) {
    //   globalPhase = globalPhase * STEP_PERIOD_FAST / stepPeriod;
    //   stepPeriod = STEP_PERIOD_FAST;
    // } else if (hasCode(IR_COMMAND_VERY_FAST_CODES, results.value)) {
    //   globalPhase = globalPhase * STEP_PERIOD_VERY_FAST / stepPeriod;
    //  stepPeriod = STEP_PERIOD_VERY_FAST;
   // }
    // EN: Once a code has been decoded, the resume() method must be called to resume receiving codes.

//    irrecv.resume();
  //}
  
  if (isAttached) {
    LeftServo.write(LEFT_SERVO_ZERO_VALUE + getAngle(amplitudeLeftServo, globalPhase, angleShiftLeftServo));
    RightServo.write(RIGHT_SERVO_ZERO_VALUE + getAngle(amplitudeRightServo, globalPhase, angleShiftRightServo));
    CentralServo.write(CENTRAL_SERVO_ZERO_VALUE + getAngle(CENTRAL_SERVO_AMPLITUDE, globalPhase, angleShiftCentralServo));
  }

 // delay(10);
}

