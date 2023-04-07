#include <WiiChuck.h>
#include <Wire.h>
#include <Servo.h>

// Comunicação via protocolo I2C COM Nunchuck: A4 (SDA), A5 (SCL)
Nunchuck nunchuck;

// Declaração de 6 servos
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;

// Valores de cada um dos servos
int countPinca = 90;
int countGiroPinca = 90;
int countElevacaoPinca = 90;
int countAntebraco = 90;
int countBraco = 90;
int countGiroBase = 90;

//Contantes
#define MAX_PINCA 180
#define MIN_PINCA 0
#define MAX_GIRO_PINCA 180
#define MIN_GIRO_PINCA 0
#define MAX_ELEVACAO_PINCA 180
#define MIN_ELEVACAO_PINCA 0
#define MAX_ANTEBRACO 180
#define MIN_ANTEBRACO 0
#define MAX_BRACO 180
#define MIN_BRACO 0
#define MAX_GIRO_BASE 180
#define MIN_GIRO_BASE 0

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  Serial.println("*** Braco Articulado ***");

  nunchuck.begin();



  
 
  
  
   myservo1.attach(5); // pinca
   myservo1.write(countPinca);
   delay(500);
   
   myservo2.attach(6); // giro pinca
   myservo2.write(countGiroPinca);
   delay(500);

   myservo3.attach(8);   
   myservo3.write(countElevacaoPinca);
   delay(500);
   
   myservo4.attach(9);
   myservo4.write(countAntebraco);
   delay(500);
   
   myservo5.attach(11);
   myservo5.write(countBraco);
   delay(500);
   
   myservo6.attach(12); // BASE
   myservo6.write(countGiroBase);
   delay(500);
   
  
  nunchuck.printMaps(Serial);

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

// Esta funcao ira ajustar o angulo, de acordo com a posição do Joystrick selecionada pelo usuário
int AjustarAngulo(int joyPos, int anguloAtual) {
  
  int var = ObterIncremento(joyPos); 

  //Pistar o LED do arduino sempre que o valor do incremento for diferente de zero, ou seja, movimento a direita ou a esquerda.
  if (var == 0 ){
       digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
   } else {
         digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
   }

//incrementar ou decrementar angulo do servo, respeitando os limites maximos (180) e minimos (0)
  if (anguloAtual<=180&&anguloAtual>=0){
     anguloAtual = anguloAtual + var;
  }
  else if (anguloAtual>=180)
  {
    anguloAtual = 180;
  }
  else if (anguloAtual<0)
  {
    anguloAtual = 0;
  }

   return anguloAtual;
}

void loop() {

  // Realiza a leitura dos dados (comandos) do Nunchuck
  nunchuck.readData();

  //Imprime o input
  nunchuck.printInputs(Serial);

   // Armazenha as variáveis de X e Y do controle.
  int joyX = nunchuck.getJoyX();
  int joyY = nunchuck.getJoyY();

// Armazenha as veriáveis do acelerometro
  int accelX = (int)nunchuck.getAccelX();
//  int accelY = (int)nunchuck.getAccelY();
//  int accelZ = (int)nunchuck.getAccelZ();

  bool buttonZ = nunchuck.checkButtonZ();
  bool buttonC = nunchuck.checkButtonC();


  if (accelX < 450) {
    countGiroPinca = constrain(AjustarAngulo(200, countGiroPinca), MIN_GIRO_PINCA, MAX_GIRO_PINCA); 
      Serial.print("OI");
      Serial.print(" | ");
  }

  if (accelX > 570) {
    countGiroPinca = constrain(AjustarAngulo(-200, countGiroPinca), MIN_GIRO_PINCA, MAX_GIRO_PINCA); 
          Serial.print("OO");
      Serial.print(" | ");
  }
 
  if (!buttonZ & !buttonC) {
     countGiroBase = constrain(AjustarAngulo(joyX, countGiroBase), MIN_GIRO_BASE, MAX_GIRO_BASE);
     countBraco = constrain(AjustarAngulo(joyY, countBraco), MIN_BRACO, MAX_BRACO);
  }


  if (!buttonZ & buttonC) {
     countAntebraco = constrain(AjustarAngulo(joyX, countAntebraco), MIN_ANTEBRACO, MAX_ANTEBRACO);
     countElevacaoPinca = constrain(AjustarAngulo(joyY, countElevacaoPinca), MIN_ELEVACAO_PINCA, MAX_ELEVACAO_PINCA);
  }

  if (buttonZ & !buttonC) {
     countGiroPinca = constrain(AjustarAngulo(joyX, countGiroPinca), MIN_GIRO_PINCA, MAX_GIRO_PINCA); 
     countPinca = constrain(AjustarAngulo(joyY, countPinca), MIN_PINCA, MAX_PINCA);
  }


//## DEBUG

  Serial.print(countPinca);
  Serial.print(" | ");
  Serial.print(countGiroPinca);
  Serial.print(" | ");
  Serial.print(countElevacaoPinca);
  Serial.print(" | ");
  Serial.print(countAntebraco);
  Serial.print(" | ");
  Serial.print(countBraco);
  Serial.print(" | ");
  Serial.println(countGiroBase);


   myservo1.write(countPinca);
   myservo2.write(countGiroPinca);
   myservo3.write(countElevacaoPinca);
   myservo4.write(countAntebraco);
   myservo5.write(countBraco);
   myservo6.write(countGiroBase);

  delay(50);
}
