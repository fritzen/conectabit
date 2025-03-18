#define SENSOR_A_PIN A6
#define SENSOR_B_PIN A7
#define SENSOR_C_PIN A5  // Novo sensor central

//L293D
//Motor A
#define LEFT_MOTOR_PIN_1 5
#define LEFT_MOTOR_PIN_2 6
//Motor B
#define RIGHT_MOTOR_PIN_1 10
#define RIGHT_MOTOR_PIN_2 9

#define SENSOR_THRESHOLD 520
#define OBSTACLE_THRESHOLD 500  // Ajuste este valor conforme a sensibilidade do sensor

void LEFT(void);
void RIGHT(void);
void STOP(void);
void BREAK(void);
void FORWARD(void);
void BACK(void);
void LEFT2(void);
void RIGHT2(void);
void turnAround(void);  // Nova função para dar meia volta

short sensor_a;
short sensor_b;
short sensor_c;  // Novo sensor central para obstáculo

#define BLACK 0
#define WHITE 1
#define OBSTACLE_DETECTED 1
#define NO_OBSTACLE 0

// Variável para controle de obstáculo
bool avoidingObstacle = false;

void setup()
{
  pinMode(LEFT_MOTOR_PIN_1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN_2, OUTPUT);
  
  pinMode(RIGHT_MOTOR_PIN_1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN_2, OUTPUT);
  
  pinMode(SENSOR_A_PIN, INPUT_PULLUP);
  pinMode(SENSOR_B_PIN, INPUT_PULLUP);
  pinMode(SENSOR_C_PIN, INPUT_PULLUP);  // Configure o novo sensor
  
  Serial.begin(9600);                   
  while (!Serial);

  BREAK();

  Serial.println("START!");
}

void readSensors() {
  sensor_a = analogRead(SENSOR_A_PIN);
  sensor_b = analogRead(SENSOR_B_PIN);
  sensor_c = analogRead(SENSOR_C_PIN);  // Leitura do sensor central
  
  Serial.print(sensor_a);
  Serial.print("*");
  Serial.print(sensor_b);
  Serial.print("*");
  Serial.print(sensor_c);  // Imprimir valor do sensor central
  Serial.print("*");

  // Processamento do sensor A e B (seguidor de linha)
  if (sensor_a < SENSOR_THRESHOLD) {
    sensor_a = WHITE;
  } else {
    sensor_a = BLACK;
  }

  if (sensor_b < SENSOR_THRESHOLD) {
    sensor_b = WHITE;
  } else {
    sensor_b = BLACK;
  }
  
  // Processamento do sensor C (obstáculo)
  if (sensor_c > OBSTACLE_THRESHOLD) {
    sensor_c = OBSTACLE_DETECTED;
    Serial.println("Obstacle detected!");
  } else {
    sensor_c = NO_OBSTACLE;
  }

  Serial.println("*");
}

void loop() 
{ 
  readSensors();
  
  // Verificar se está em processo de desvio de obstáculo
  if (avoidingObstacle) {
    // Verificar se o sensor direito não detecta mais preto
    if (sensor_b != BLACK) {
      avoidingObstacle = false;
      STOP();
      delay(200);
      Serial.println("Obstacle avoided!");
    } else {
      // Continua girando
      LEFT();
    }
    return;  // Não executa o resto do código enquanto estiver evitando obstáculo
  }
  
  // Verifica sensor de obstáculo primeiro
  if (sensor_c == OBSTACLE_DETECTED) {
    handleObstacle();  // Inicia o processo de desvio
    return;
  }
  
  // Lógica do seguidor de linha, se não houver obstáculo
  if (sensor_a == BLACK) {
    LEFT2();
  } else if (sensor_b == BLACK) {            
    RIGHT2();
  } else {
    FORWARD();  // Se não detectar linha preta em nenhum sensor, segue em frente
  }
}

void handleObstacle() {
  BREAK(); // Parar
  delay(200); // Pequena pausa
  
  BACK(); // Ré
  delay(500); // Recuar por meio segundo
  
  BREAK(); // Parar
  delay(200); // Pequena pausa
 
  LEFT(); // Iniciar giro
  delay(600); // Dar um tempo inicial de giro
  
  // Continuar girando até que o sensor direito não detecte preto
  avoidingObstacle = true;
  Serial.println("Avoiding obstacle!");
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

void RIGHT2(void)
{
  digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN_2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN_2, LOW);
}

void RIGHT(void)
{
  digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN_2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN_1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN_2, LOW);
}

void LEFT2(void)
{
  digitalWrite(LEFT_MOTOR_PIN_1, LOW);
  digitalWrite(LEFT_MOTOR_PIN_2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN_2, HIGH);
}

void LEFT(void)
{
  digitalWrite(LEFT_MOTOR_PIN_1, LOW);
  digitalWrite(LEFT_MOTOR_PIN_2, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN_2, HIGH);
}

void STOP(void)
{
  digitalWrite(LEFT_MOTOR_PIN_1, LOW);
  digitalWrite(LEFT_MOTOR_PIN_2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN_2, LOW);
}

void BREAK(void)
{
  digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN_2, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN_1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN_2, HIGH);
}