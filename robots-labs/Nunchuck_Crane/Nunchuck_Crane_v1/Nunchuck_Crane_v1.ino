//https://github.com/datacute/WiiChuck
#include <WiiChuck.h>

#include <Stepper.h>
#include <Wire.h>
#include <Servo.h>

//Contantes
#define MAX_SERVO 180
#define MIN_SERVO 0
#define PIN_ELETROIMA 13
#define PIN_SERVO 3
#define STEPS  32


// Comunicação via protocolo I2C COM Nunchuck: A4 (SDA), A5 (SCL)
Nunchuck nunchuck(SDA, SCL);

// Declaração de 6 servos
Servo myservo;
 
// Valores de cada um dos servos
int countServo = 90;
bool eletroImaState = false;

//Stepper stepper(STEPS, 2, 4, 3, 5);

Stepper stepper(STEPS, 9, 11, 10, 12);

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(PIN_ELETROIMA, OUTPUT);
  
  
  Serial.begin(115200);
  Serial.println("*** Guindaste Articulado ***");

  nunchuck.begin();

  myservo.attach(PIN_SERVO);
  myservo.write(countServo);

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
 // nunchuck.printInputs(Serial);

   // Armazenha as variáveis de X e Y do controle.
  int joyX = nunchuck.getJoyX();
  int joyY = nunchuck.getJoyY();

// Armazenha as veriáveis do acelerometro
  int accelX = (int)nunchuck.getAccelX();
 
 //NAO USADOS
 // int accelY = (int)nunchuck.getAccelY();
 // int accelZ = (int)nunchuck.getAccelZ(); 

  bool buttonZ = nunchuck.checkButtonZ();
  bool buttonC = nunchuck.checkButtonC();

  countServo = constrain(AjustarAngulo(joyY, countServo), MIN_SERVO, MAX_SERVO);
  myservo.write(countServo);


   
   
  if (accelX < 450) {
    stepper.step(1);
  }

  if (accelX > 570) {
    stepper.step(-1);
  }

  if (buttonZ)
  {
    digitalWrite(PIN_ELETROIMA, HIGH);
    eletroImaState = true;
  } else {
    digitalWrite(PIN_ELETROIMA, LOW);
    eletroImaState = false;
  }
  if (buttonC) {
    eletroImaState = !eletroImaState;
    digitalWrite(PIN_ELETROIMA, eletroImaState);
  }
  

//## DEBUG

 // Serial.print(eletroImaState);
 // Serial.println(" | ");



  delay(5);
}
