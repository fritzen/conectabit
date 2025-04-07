#include <Servo.h>

// Definições dos pinos dos sensores
#define LEFT_SENSOR_PIN A2
#define RIGHT_SENSOR_PIN A3
#define IR_SENSOR_PIN A0  // Sensor de colisão frontal infravermelho analógico

// Pinos dos motores (L293D)
// Motor A
#define LEFT_MOTOR_PIN_1 5
#define LEFT_MOTOR_PIN_2 6
// Motor B
#define RIGHT_MOTOR_PIN_1 10
#define RIGHT_MOTOR_PIN_2 9

#define SENSOR_THRESHOLD 220
#define IR_THRESHOLD 320  // Ajuste conforme o sensor utilizado

void LEFT(void);
void RIGHT(void);
void STOP(void);

short left_sensor;
short right_sensor;
int ir_value; // Valor lido do sensor de colisão

#define BLACK  0
#define WHITE  1

void setup() {
  pinMode(LEFT_SENSOR_PIN, INPUT);
  pinMode(RIGHT_SENSOR_PIN, INPUT);
  pinMode(IR_SENSOR_PIN, INPUT);
  
  pinMode(LEFT_MOTOR_PIN_1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN_2, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN_1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN_2, OUTPUT);

  Serial.begin(9600);
  while (!Serial);
  BREAK();
  Serial.println("DUDU START!");
}

void readSensors() {
    left_sensor = analogRead(LEFT_SENSOR_PIN);
    right_sensor = analogRead(RIGHT_SENSOR_PIN);
    ir_value = analogRead(IR_SENSOR_PIN);
    
    Serial.print("Left: ");
    Serial.print(left_sensor);
    Serial.print(" | Right: ");
    Serial.print(right_sensor);
    Serial.print(" | IR: ");
    Serial.println(ir_value);

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
}


// 0 = expectedRight, 1 = expectedLeft, 2 = nothing
int expected = 0;
    

    
void DoObstacle(){

          BREAK(); //Parar
          delay(50);

 

    
}


void loop() {


    readSensors();


    // Verifica colisão pelo sensor IR
  if (ir_value < IR_THRESHOLD) {
    DoObstacle();
    return;
  }

  // Lógica do seguidor de linha
  if (left_sensor == BLACK && right_sensor == BLACK) {
    FORWARD();
  }
  else if (left_sensor == BLACK && right_sensor == WHITE) {
    LEFT2();
    expected = 1; // Caso perca a linha, busca pela esquerda
  }
  else if (left_sensor == WHITE && right_sensor == BLACK) {
    RIGHT2();
    expected = 0; // Caso perca a linha, busca pela direita
  }
  else { // Ambos sensores brancos (linha perdida)
    if (expected == 0) {
      RIGHT2();
    }
    else if (expected == 1) {
      LEFT2();
    }
    else {
      STOP();
    }
  }
  
  delay(50);

}

void FORWARD() {   
    digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, HIGH); 
}

void BACK() {
    digitalWrite(LEFT_MOTOR_PIN_1, LOW);
    digitalWrite(LEFT_MOTOR_PIN_2, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN_1, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN_2, LOW);
}

void RIGHT2(void) {
    digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, LOW);
}

void RIGHT(void) {
    digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_1, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN_2, LOW);
}

void LEFT2(void) {
    digitalWrite(LEFT_MOTOR_PIN_1, LOW);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, HIGH);
}

void LEFT(void) {
    digitalWrite(LEFT_MOTOR_PIN_1, LOW);
    digitalWrite(LEFT_MOTOR_PIN_2, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, HIGH);
}

void STOP(void) {
    digitalWrite(LEFT_MOTOR_PIN_1, LOW);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, LOW);
}

void BREAK(void) {
    digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN_2, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN_1, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN_2, HIGH);
}
