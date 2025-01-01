#include <Servo.h>          //Servo motor library. This is standard library

//#include <NewPing.h>        //Ultrasonic sensor function library. You must install this library

#include <IRremote.h>
int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;

//our L298N control pins
const int LeftMotorForward = 7;
const int LeftMotorBackward = 6;
const int RightMotorForward = 4;
const int RightMotorBackward = 5;

//sensor pins
#define trig_pin A1 //analog input 1
#define echo_pin A2 //analog input 2

#define MIN_DISTANCE  900


boolean goesForward = false;
int distance = 100;

bool automatic = true;
int command_time = 100;

void setup(){

//const int out=12; trig_pin
//const int in=13;   echo_pin


    pinMode(echo_pin, INPUT); //remember: the Echo pin is the microphone pin
    pinMode(trig_pin, OUTPUT); //remember: the Trig pin is the speaker pin

  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);
  

  distance = readPing();
  delay(100);

  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  irrecv.enableIRIn();
  
  Serial.println("Enabled IRin");

  randomSeed(analogRead(0));
  
}

void loop(){

  delay(50);

if (automatic) {
  
  if (distance <= MIN_DISTANCE){
    moveStop();
    delay(300);
    moveBackward();    
    delay(400);


    int rnd = random(1);

    if (rnd == 1){
      moveStop();  
      turnRight();  
    } else {
      moveStop();
      turnLeft(); 
    }

    delay(50);
  }
  else{
    moveForward(); 
  }
  
  distance = readPing();
}else {
  moveStop();
}

  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    Serial.println("-");


    if (results.value == 0xFF629D) //UP
    {
        moveForward();
        delay(command_time);
        moveStop();
    }
    if (results.value == 0xFF22DD) //LEFT
    {
        turnLeft();
        delay(command_time);
        moveStop();        
    }
    if (results.value == 0xFFC23D) //RIGHT
    {
        turnRight();
        delay(command_time);
        moveStop();          
    }
    if (results.value == 0xFFA857) //DOWN FFA857
    {
        moveBackward2();
        delay(command_time);
        moveStop();   
    }
    if (results.value == 0xFF02FD) //OK
    {
       automatic = !automatic;
       moveStop();
    }
    if (results.value == 0xFF6897) //1
    {
       command_time = 100;
    }
    if (results.value == 0xFF9867) //2
    {
       command_time = 200;         
    }
    if (results.value == 0xFFB04F) //3
    {
       command_time = 300;
    }
    if (results.value == 0xFF30CF) //4
    {
       command_time = 400;
    }
    if (results.value == 0xFF18E7) //5
    {
       command_time = 500;
    }
    if (results.value == 0xFF7A85) //6
    {
       command_time = 600;
    }
    if (results.value == 0xFF10EF) //7
    {
       command_time = 700;
    }
    if (results.value == 0xFF38C7) //8
    {
       command_time = 800;
    }
    if (results.value == 0xFF5AA5) //9
    {
       command_time = 900;
    }
    if (results.value == 0xFF4AB5) //0
    {
       command_time = 1000;
    }
    if (results.value == 0xFF42BD) //*
    {
        turnRight();
        delay(1000);
        turnLeft();
        delay(1000);
        moveStop();
    }
    if (results.value == 0xFF52AD) //#
    {
      for (int i = 0; i <= 10; i++) {
        moveBackward();
        delay(500);
        moveForward();
        delay(500);        
      }
     
        moveStop();        
    }


    irrecv.resume();
  }


}


  
int readPing(){

  
  
  long dur;
  long dis;

  digitalWrite(trig_pin,LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin,LOW);

  dur=pulseIn(echo_pin,HIGH);


  if (dur==0){
    dur=250;
  }
  
  Serial.println(String(dur));

  delay(100);


  return dur;
}

void moveStop(){
  
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

void moveBackward(){ //Trocado com moveForward

  if(!goesForward){

    goesForward=true;
    
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);
  
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW); 
  }
}

void moveBackward2(){ //Trocado com moveForward

    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);
  
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW); 
}
void moveForward(){

  goesForward=false;

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
}

void turnRight(){

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

void turnLeft(){

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
