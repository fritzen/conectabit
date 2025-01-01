const int TRIG_PIN = 7; // Pino do trigger do sensor HC-SR04 (conectado ao pino digital 4 do Arduino)
const int ECHO_PIN = 6; // Pino do echo do sensor HC-SR04 (conectado ao pino digital 5 do Arduino)



void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Dispara o pulso ultrassônico
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(15);
  digitalWrite(TRIG_PIN, LOW);

 // Mede o tempo do retorno do eco
  long duration = pulseIn(ECHO_PIN, HIGH, 30000) ; // Timeout de 30 milissegundos

  // Verifica se o tempo de retorno é válido (diferente de 0)
  if (duration != 0) {
    // Calcula a distância em milímetros
    float distance_mm = duration * 0.034 / 2 * 10;

    // Exibe a distância no monitor serial em milímetros
    Serial.print("Distancia: ");
    Serial.print(distance_mm);
    Serial.println(" mm");
  } else {
    // Caso o pulso ultrassônico não retorne a tempo, exibe uma mensagem de erro
    Serial.println("Erro: Leitura do sensor excedeu o tempo limite.");
  }

  delay(200); // Intervalo entre as medições
}