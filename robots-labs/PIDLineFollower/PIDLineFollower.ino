// IR Sensors
#define SENSOR1 A0      // Left most sensor
#define SENSOR2 A1
#define SENSOR3 A2
#define SENSOR4 A3
#define SENSOR5 A4      // Right most sensor

// Initial Values of Sensors
int sensor[5] = {0, 0, 0, 0, 0};


#define A1A 3//speed PWM port
#define A1B 4 

#define B1A 5 //speed PWM port
#define B1B 6

//Initial Speed of Motor
int initial_motor_speed = 140;

// PID Constants
float Kp = 25;
float Ki = 0;
float Kd = 15;

float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;

int flag = 0;

void setup()
{
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
  pinMode(SENSOR5, INPUT);

  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  pinMode(B1A, OUTPUT);
  pinMode(B1B, OUTPUT);

  Serial.begin(9600);
  delay(500);
  Serial.println("Started !!");
  delay(1000);
}
void loop()
{
  read_sensor_values();
  Serial.print("ERROR: ");
  Serial.print(error);
  Serial.print("\t");
  
  if (error == 100) {               // Make left turn untill it detects straight path
    //Serial.print("\t");
    //Serial.println("Left");
    do {
      read_sensor_values();
      analogWrite(A1A, 110); //Left Motor Speed
      analogWrite(B1A, 90); //Right Motor Speed
      sharpLeftTurn();
    } while (error != 0);

  } else if (error == 101) {          // Make right turn in case of it detects only right path (it will go into forward direction in case of staright and right "|--")
                                      // untill it detects straight path.
    Serial.print("\t");
    Serial.println("RIGHT");
    analogWrite(A1A, 110); //Left Motor Speed
    analogWrite(B1A, 90); //Right Motor Speed
    forward();
    delay(200);
    stop_bot();
    read_sensor_values();
    if (error == 102) {
      do {
        analogWrite(A1A, 110); //Left Motor Speed
        analogWrite(B1A, 90); //Right Motor Speed
        sharpRightTurn();
        read_sensor_values();
      } while (error != 0);
    }
  } else if (error == 102) {        // Make left turn untill it detects straight path
    //Serial.print("\t");
    //Serial.println("Sharp Left Turn");
    do {
      analogWrite(A1A, 110); //Left Motor Speed
      analogWrite(B1A, 90); //Right Motor Speed
      sharpLeftTurn();
      read_sensor_values();
      if (error == 0) {
        stop_bot();
        delay(200);
      }
    } while (error != 0);
  } else if (error == 103) {        // Make left turn untill it detects straight path or stop if dead end reached.
    if (flag == 0) {
      analogWrite(A1A, 110); //Left Motor Speed
      analogWrite(B1A, 90); //Right Motor Speed
      forward();
      delay(200);
      stop_bot();
      read_sensor_values();
      if (error == 103) {     /**** Dead End Reached, Stop! ****/
        stop_bot();
        flag = 1;
      } else {        /**** Move Left ****/
        analogWrite(A1A, 110); //Left Motor Speed
        analogWrite(B1A, 90); //Right Motor Speed
        sharpLeftTurn();
        delay(200);
        do {
          Serial.print("\t");
          Serial.println("LEFT ");
          read_sensor_values();
          analogWrite(A1A, 110); //Left Motor Speed
          analogWrite(B1A, 90); //Right Motor Speed
          sharpLeftTurn();
        } while (error != 0);
      }
    }
  } else {
    calculate_pid();
    motor_control();
  }

  Serial.println("");
}

void read_sensor_values()
{
  sensor[0] = !digitalRead(SENSOR1);
  sensor[1] = !digitalRead(SENSOR2);
  sensor[2] = !digitalRead(SENSOR3);
  sensor[3] = !digitalRead(SENSOR4);
  sensor[4] = !digitalRead(SENSOR5);

  
    Serial.print(sensor[0]);
    Serial.print("  ");
    Serial.print(sensor[1]);
    Serial.print("  ");
    Serial.print(sensor[2]);
    Serial.print("  ");
    Serial.print(sensor[3]);
    Serial.print("  ");
    Serial.print(sensor[4]);
    Serial.print("  ");
  if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
    error = 4;
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
    error = 3;
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0)&& (sensor[4] == 0))
    error = 2;
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0)&& (sensor[4] == 0))
    error = 1;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0)&& (sensor[4] == 0))
    error = 0;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1)&& (sensor[4] == 0))
    error = -1;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1)&& (sensor[4] == 0))
    error = -2;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1)&& (sensor[4] == 1))
    error = -3;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0)&& (sensor[4] == 1))
    error = -4;   
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0)&& (sensor[4] == 0)) // Turn robot left side
    error = 200;
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1)&& (sensor[4] == 0)) // Turn robot right side
    error = 201;
  //else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0)&& (sensor[4] == 0)) // Make U turn
    //error = 202;
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1)&& (sensor[4] == 1)) // Turn left side or stop
    error = 203;
}

void calculate_pid()
{
  P = error;
  I = I + previous_I;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);

  previous_I = I;
  previous_error = error;
}

void motor_control()
{
  // Calculating the effective motor speed:
  int left_motor_speed = initial_motor_speed - PID_value;
  int right_motor_speed = initial_motor_speed + PID_value;

  // The motor speed should not exceed the max PWM value
  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);

  Serial.print("\tPID: ");
  Serial.print(PID_value);
  Serial.print("\tSPEED_A: ");
  Serial.print(left_motor_speed);
  Serial.print("\tSPEED_B: ");
  Serial.print(right_motor_speed);


 analogWrite(A1A, right_motor_speed); //Right Motor Speed
 analogWrite(B1A, left_motor_speed); //Left Motor Speed
 
  //following lines of code are to make the bot move forward
  forward();
}

void forward()
{
  /*The pin numbers and high, low values might be different depending on your connections */
  Serial.print("\t");
  Serial.print("FORWARD");
  

  digitalWrite(A1B, LOW);
  digitalWrite(B1B, LOW);
}

void sharpRightTurn() {
  /*The pin numbers and high, low values might be different depending on your connections */
//  digitalWrite(A1A, LOW);
//  digitalWrite(A1B, HIGH);
//  digitalWrite(B1A, HIGH);
//  digitalWrite(B1B, LOW);
}
void sharpLeftTurn() {
  /*The pin numbers and high, low values might be different depending on your connections */
//  digitalWrite(A1A, HIGH);
//  digitalWrite(A1B, LOW);
//  digitalWrite(B1A, LOW);
//  digitalWrite(B1B, HIGH);
}
void stop_bot()
{
  digitalWrite(A1A, HIGH);
  digitalWrite(A1B, HIGH);
  digitalWrite(B1A, HIGH);
  digitalWrite(B1B, HIGH);
}
