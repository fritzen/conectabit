// Pino de trigger do sensor
#define TRIGGER_PIN 2
// Pino de echo do sensor
#define ECHO_PIN 3
const int POWER_PIN = 4;


void setup() {
  
  // Configure os pinos do sensor
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);


  // Inicialize a comunicação serial
  Serial.begin(9600);

  // Configure o pino como saída
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
  
  
}

void loop() {
  // Acione o pulso de disparo
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Meça a duração do pulso de eco
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Converta a duração em distância em centímetros
  float distance = duration * 0.034 / 2;

  // Imprima a distância no monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Aguarde um curto período antes de realizar a próxima leitura
  delay(100);
}