#include <musical_notes.h>

int motorDer1=2;//El pin 7 a In1 del L298N
int motorDer2=3;//El pin 6 a In2 del L298N
int motorIzq1=4;//El pin 5 a In3 del L298N
int motorIzq2=5;//El pin 4 a In4 del L298N
boolean moviendoseAdelante= false;
boolean ultGiroIzquierda = false;

long tiempo;

float distanciaIzq;
float distanciaDer;

int disparadorIzq = 9;   // triger izquierdo
int entradaIzq = 8;      // echo izquierdo
int disparadorDer = 11;  // triger derecho 
int entradaDer = 10;     // echo derecho

int speakerPin = 12; // Buzzer conectado a digital pin 1

void setup() {
  pinMode(motorDer1, OUTPUT); 
  pinMode(motorDer2, OUTPUT);
  pinMode(motorIzq1, OUTPUT); 
  pinMode(motorIzq2, OUTPUT); 
  pinMode(disparadorIzq, OUTPUT);
  pinMode(entradaIzq, INPUT);
  pinMode(disparadorDer, OUTPUT);
  pinMode(entradaDer, INPUT);
  pinMode(speakerPin, OUTPUT);
  
  Serial.begin(9600);
}

void adelante(){ 
  digitalWrite(motorDer1,HIGH);
  digitalWrite(motorDer2,LOW);
  digitalWrite(motorIzq1,HIGH);
  digitalWrite(motorIzq2,LOW);
}
void atras(){ 
  digitalWrite(motorDer1,LOW);
  digitalWrite(motorDer2,HIGH);
  digitalWrite(motorIzq1,LOW);
  digitalWrite(motorIzq2,HIGH);
}
void giraDerecha(){ 
  digitalWrite(motorDer1,HIGH);
  digitalWrite(motorDer2,LOW);
  digitalWrite(motorIzq1,LOW);
  digitalWrite(motorIzq2,HIGH);
}
void giraIzquierda(){ 
  digitalWrite(motorDer1,LOW);
  digitalWrite(motorDer2,HIGH);
  digitalWrite(motorIzq1,HIGH);
  digitalWrite(motorIzq2,LOW);
}
void parar(){ 
  digitalWrite(motorDer1,LOW);
  digitalWrite(motorDer2,LOW);
  digitalWrite(motorIzq1,LOW);
  digitalWrite(motorIzq2,LOW);
}

void pararIzquierda()
{
  digitalWrite(motorIzq1,LOW);
  digitalWrite(motorIzq2,LOW);
}

void pararDerecha()
{
  digitalWrite(motorDer1,LOW);
  digitalWrite(motorDer2,LOW);
}

void adelanteDerecha()
{
  digitalWrite(motorDer1,HIGH);
  digitalWrite(motorDer2,LOW);
}

void adelanteIzquierda()
{
  digitalWrite(motorIzq1,HIGH);
  digitalWrite(motorIzq2,LOW);
}

void giroIzqSensor()
{ 

  boolean noHayEspacio = true;
  float distanciaGiro;
  
  pararIzquierda();
  delay(100);
  
  while(noHayEspacio)
  {  
    distanciaGiro = calculaDistancia(disparadorIzq, entradaIzq);
    if (distanciaGiro > 30)
    {
      noHayEspacio = false;
    }
    delay(50);
  }
  adelanteIzquierda();
}

void giroDerSensor(){ 

  boolean noHayEspacio = true;
  float distanciaGiro;
  
  pararDerecha();
  delay(100);
  
  while(noHayEspacio)
  {  
    distanciaGiro = calculaDistancia(disparadorDer, entradaDer);
    if (distanciaGiro > 30)
    {
      noHayEspacio = false;
    }
    delay(50);
  }
  adelanteDerecha();
}

float calculaDistancia(int disparador, int entrada)
{
  float distanciaCM;
  // lanzamos un pequeño pulso para activar el sensor
  digitalWrite(disparador, HIGH);
  delayMicroseconds(10);
  digitalWrite(disparador, LOW);
  
   // medimos el pulso de respuesta
  tiempo = (pulseIn(entrada, HIGH)/2); // dividido por 2 por que es el 
                                       // tiempo que el sonido tarda
                                       // en ir y en volver
  // ahora calcularemos la distancia en cm
  // sabiendo que el espacio es igual a la velocidad por el tiempo
  // y que la velocidad del sonido es de 343m/s y que el tiempo lo 
  // tenemos en millonesimas de segundo
  distanciaCM = float(tiempo * 0.0343);
  
  return(distanciaCM);
}

// Voy haciendo microgiros y midiendo la distancia
// si la distancia es superior a 1 metro, salgo del bucle
// y continua la ejecución.
void giroSensor(){ 

  boolean noHayEspacio = true;
  float distanciaGiroDer;
  float distanciaGiroIzq;
  
  while(noHayEspacio){
    
    if (ultGiroIzquierda == true)
    {
      giraDerecha();
    }
    else
    {
      giraIzquierda();
    }   
    delay(200);
    parar();
    delay(200);
    
    distanciaGiroDer = calculaDistancia(disparadorDer, entradaDer);  
    distanciaGiroIzq = calculaDistancia(disparadorIzq, entradaIzq); 

    if (distanciaGiroDer > 50 && distanciaGiroIzq > 50)
    {
      noHayEspacio = false;
    }    
  }
  
  if (ultGiroIzquierda == true)
  {
    ultGiroIzquierda = false;
  }
  else
  {
    ultGiroIzquierda = true;
  }  
}

void loop()
{
  distanciaIzq = calculaDistancia(disparadorIzq, entradaIzq);
  distanciaDer = calculaDistancia(disparadorDer, entradaDer);
  //Serial.println(distanciaIzq);
  //Serial.println(distanciaDer);

  if (distanciaIzq >= 30 && distanciaDer >= 30)
  {
    if (moviendoseAdelante == false)
    {
       //Serial.println("adelanteee");
       adelante();
       moviendoseAdelante = true; 
    }
    //Serial.println("adelanteee1");
  }
  else if (distanciaIzq >= 30 && distanciaDer < 30)
  { 
    //Serial.println("entra por giro izq");
    giroIzqSensor();
  }
  else if (distanciaIzq < 30 && distanciaDer >= 30)
  {
    //Serial.println("entra por giro der");
    giroDerSensor();
  }
  else if (distanciaIzq < 30 && distanciaDer < 30)
  {
    if (moviendoseAdelante == true)
    {
      giroSensor();
      r2D2();
      moviendoseAdelante = false;
    }
  }
}

void beep (int speakerPin, float noteFrequency, long noteDuration)
{    
  int x;
  // Convert the frequency to microseconds
  float microsecondsPerWave = 1000000/noteFrequency;
  // Calculate how many HIGH/LOW cycles there are per millisecond
  float millisecondsPerCycle = 1000/(microsecondsPerWave * 2);
  // Multiply noteDuration * number or cycles per millisecond
  float loopTime = noteDuration * millisecondsPerCycle;
  // Play the note for the calculated loopTime.
  for (x=0;x<loopTime;x++)   
          {   
              digitalWrite(speakerPin,HIGH); 
              delayMicroseconds(microsecondsPerWave); 
              digitalWrite(speakerPin,LOW); 
              delayMicroseconds(microsecondsPerWave); 
          } 
}  

void r2D2(){
          beep(speakerPin, note_A7,100); //A 
          beep(speakerPin, note_G7,100); //G 
          beep(speakerPin, note_E7,100); //E 
          beep(speakerPin, note_C7,100); //C
          beep(speakerPin, note_D7,100); //D 
          beep(speakerPin, note_B7,100); //B 
          beep(speakerPin, note_F7,100); //F 
          beep(speakerPin, note_C8,100); //C 
          beep(speakerPin, note_A7,100); //A 
          beep(speakerPin, note_G7,100); //G 
          beep(speakerPin, note_E7,100); //E 
          beep(speakerPin, note_C7,100); //C
          beep(speakerPin, note_D7,100); //D 
          beep(speakerPin, note_B7,100); //B 
          beep(speakerPin, note_F7,100); //F 
          beep(speakerPin, note_C8,100); //C 
}


void closeEncounters() {
          beep(speakerPin, note_Bb5,300); //B b
          delay(50);
          beep(speakerPin, note_C6,300); //C
          delay(50);
          beep(speakerPin, note_Ab5,300); //A b
          delay(50);
          beep(speakerPin, note_Ab4,300); //A b
          delay(50);
          beep(speakerPin, note_Eb5,500); //E b   
          delay(500);     
          
          beep(speakerPin, note_Bb4,300); //B b
          delay(100);
          beep(speakerPin, note_C5,300); //C
          delay(100);
          beep(speakerPin, note_Ab4,300); //A b
          delay(100);
          beep(speakerPin, note_Ab3,300); //A b
          delay(100);
          beep(speakerPin, note_Eb4,500); //E b   
          delay(500);  
          
          beep(speakerPin, note_Bb3,300); //B b
          delay(200);
          beep(speakerPin, note_C4,300); //C
          delay(200);
          beep(speakerPin, note_Ab3,300); //A b
          delay(500);
          beep(speakerPin, note_Ab2,300); //A b
          delay(550);
          beep(speakerPin, note_Eb3,500); //E b      
}
