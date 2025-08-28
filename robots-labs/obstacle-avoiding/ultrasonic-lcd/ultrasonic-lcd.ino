#include <Servo.h>   //Servo motor library. This is standard library
#include <NewPing.h> //Ultrasonic sensor function library. You must install this library
#include "U8glib.h"
#include "images.h"

//our L298N control pins
const int LeftMotorForward = 7;
const int LeftMotorBackward = 6;
const int RightMotorForward = 4;
const int RightMotorBackward = 5;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0); // I2C / TWI

int tempoSorriso = 1;

//sensor pins
#define trig_pin A1 //analog input 1
#define echo_pin A2 //analog input 2

#define maximum_distance 200
boolean goesForward = false;
int distance = 100;

NewPing sonar(trig_pin, echo_pin, maximum_distance); //sensor function

void drawSorriso()
{

  u8g.firstPage();

  do
  {
    //u8g.drawXBMP(0, 0, 16, 64, space4);
    u8g.drawBitmapP(1, 0, 16, 64, open);
  } while (u8g.nextPage());
}

void drawSorrisoClose()
{
  u8g.firstPage();

  do
  {
    //u8g.drawXBMP(0, 0, 16, 64, space4);
    u8g.drawBitmapP(1, 0, 16, 64, close);
  } while (u8g.nextPage());
}

void setup()
{

  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  distance = readPing();
  delay(100);

  Serial.begin(9600);

  randomSeed(analogRead(0));
}

void loop()
{

  delay(1);

  if (distance <= 40)
  {
    drawSorrisoClose();
    moveStop();
    delay(300);
    moveBackward();
    delay(400);

    int rnd = random(3);
    Serial.println(rnd);

    if (rnd == 1)
    {
      moveStop();
      turnRight();
    }
    else
    {
      moveStop();
      turnLeft();
    }

    delay(50);
  }
  else
  {
    drawSorriso();
    moveForward();
  }

  distance = readPing();
}

int readPing()
{
  delay(50);
  int cm = sonar.ping_cm();
  if (cm == 0)
  {
    cm = 250;
  }

  //Serial.println(cm);
  return cm;
}

void moveStop()
{

  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

void moveBackward()
{ //Trocado com moveForward

  if (!goesForward)
  {

    goesForward = true;

    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);

    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW);
  }
}

void moveForward()
{

  goesForward = false;

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);

  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
}

void turnRight()
{

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);

  delay(500);

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}

void turnLeft()
{

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);

  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  delay(500);

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}
