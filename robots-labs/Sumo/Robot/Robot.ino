// NO INÍCIO DO PROGRAMA, DEFINIMOS A QUE PINOS OS DIFERENTES DISPOSITIVOS ESTARÃO CONECTADOS
#define SENSOR_ESQUERDO 2
#define SENSOR_DIREITO 3
#define BOTAO 4 // Pino 4 e pino negativo
#define MOTOR_ESQUERDO 5
#define MOTOR_DIREITO 6
#define ECHO_PIN 16     // Sensor ultrassônico pino A2
#define TRIGGER_PIN 17  // Sensor ultrassônico pino A3

#define DISTANCIA_MAXIMA 40        // Esta será a distância de detecção do robô

// ANTES DE INICIAR O PROGRAMA, CARREGAMOS AS SEGUINTES BIBLIOTECAS PARA FACILITAR A LABOR
#include <Servo.h>

// ESTE CÓDIGO É PARA INICIALIZAR AS BIBLIOTECAS
Servo servoDireito;   // Inicializa um servo com o nome servoDireito
Servo servoEsquerdo; // Inicializa um servo com o nome servoEsquerdo

// DECLARAMOS UMA SÉRIE DE VARIÁVEIS PARA TORNAR O PROGRAMA MAIS LEGÍVEL E PRÁTICO
boolean modoCombate = false;  // Inicializamos a variável para que o robô comece parado
int estadoSensorDireito;    // Variável que armazena o estado do sensor Direito
int estadoSensorEsquerdo;  // Variável que armazena o estado do sensor Esquerdo
float distancia;           // Variável para medir a distância com o sensor ultrassônico

// SETUP FUNCIONA UMA SÓ VEZ NO INÍCIO DO PROGRAMA
// A USAREMOS PARA CONFIGURAR A FUNÇÃO DE CADA PINO
void setup() {
  pinMode(BOTAO, INPUT_PULLUP); // O botão será uma entrada
  pinMode(SENSOR_DIREITO, INPUT);  // O pino será uma entrada para ler o sensor de linha
  pinMode(SENSOR_ESQUERDO, INPUT);// O pino será uma entrada para ler o sensor de linha
  pinMode(ECHO_PIN, INPUT);     // O pino será uma entrada para ler o sensor ultrassônico
  pinMode(TRIGGER_PIN, OUTPUT); // O pino enviará sinais ultrassônicos, será uma saída.
  
  servoDireito.attach(MOTOR_DIREITO);      // O pino controlará um servo
  servoEsquerdo.attach(MOTOR_ESQUERDO);  // O pino controlará um servo
  
  // Inicialização do monitor serial
  Serial.begin(9600);
  Serial.println("ROBÔ PRONTO...");
  Serial.println("Pressione o botão para começar a luta");
}

// A FUNÇÃO LOOP SE REPETE CONSTANTEMENTE, É O CORAÇÃO DO PROGRAMA
void loop() {
  delay(50);      // Fazemos uma pequena pausa para estabilizar o programa
  lerBotao();    // Lemos o estado do botão para iniciar ou parar o robô
  lerSensores(); // Lemos os sensores ultrassônicos e de linha

  if (modoCombate == false) // O ROBÔ ESTÁ EM ESPERA
    Parado();
  else { // O ROBÔ ESTÁ EM MODO DE COMBATE
    if (estadoSensorDireito == LOW || estadoSensorEsquerdo == LOW) // DETECTA A LINHA
      Evasao();  // Esquiva-se da linha
    if (distancia != 0)  // DETECTA UM INIMIGO
      Frente(); // Ataca movendo-se para frente
    else
      GirarDireita(); // PROCURA O INIMIGO
  }

}

// ESTA FUNÇÃO LÊ O BOTÃO DO ROBÔ
void lerBotao() {
  int estadoBotao = digitalRead(BOTAO);  // Lê o estado do botão

  if (estadoBotao == LOW) {
    modoCombate = !modoCombate; // ALTERAMOS O ESTADO DO ROBÔ AO PRESSIONAR O BOTÃO
    delay(500); // Delay antirrepiquete, para que o botão não envie sinais diferentes
    if (modoCombate == true){ // ANTES DE ENTRAR EM COMBATE, ESPERAMOS 5 SEGUNDOS
      Serial.println("ENTRANDO EM MODO DE COMBATE");
      delay(4500);
    }
    if(modoCombate == false){
      Serial.println("ENTRANDO EM MODO DE REPOUSO");
    }
  }
}

// ESTA FUNÇÃO LÊ OS SENSORES DO ROBÔ
void lerSensores() {
  estadoSensorDireito = digitalRead(SENSOR_DIREITO); // Lê o sensor de linha direito
  estadoSensorEsquerdo = digitalRead(SENSOR_ESQUERDO); // Lê o sensor de linha esquerdo
  // Mede a distância com o sensor ultrassônico
  distancia = medirDistancia();
}

// FUNÇÃO PARA MEDIR A DISTÂNCIA COM O SENSOR HCSR04
float medirDistancia() {
  // Limpa o pino de disparo (trigger)
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);

  // Emite um pulso de 10 microssegundos no pino de disparo
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Mede a duração do pulso no pino de eco (echo)
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calcula a distância em centímetros com base na velocidade do som (aproximadamente 343 m/s)
  float distancia = (duration * 0.0343) / 2.0; // Dividido por 2 para obter a distância de ida e volta

  return distancia;
}

// A PARTIR DAQUI, SÃO TODAS AS MANOBRAS DE MOVIMENTO
void GirarDireita() {
  Serial.println("Procurando Inimigo...");
  servoDireito.write(120);
  servoEsquerdo.write(120);
  delay(50);
}

void GirarEsquerda() {
  servoDireito.write(0);
  servoEsquerdo.write(0);
  delay(50);
}

void Frente() {
  Serial.println("ATAQUE");
  servoDireito.write(0);
  servoEsquerdo.write(180);
  delay(100);
}

void Atras() {
  servoDireito.write(180);
  servoEsquerdo.write(0);
  delay(50);
}

void Parado() {
  servoDireito.write(90);
  servoEsquerdo.write(90);
  delay(50);
}

void Evasao() {
  Parado();
  delay(100);
  Serial.println("Esquiva");
  Atras();
  delay(1000);
}
