// Projeto Curto Circuito - A4988 & Arduino: Controle para motores Bipolar
//---- Pinos de controle A4988--- 
#define ENA 13 // Enable ativa os drivers do motor
#define MS1 12 // Seleciona a resolução do passo
#define MS2 11 // Seleciona a resolução do passo
#define MS3 10 // Seleciona a resolução do passo 
#define RST 9 // Desativa os drivers de saída
#define SLP 8 // Inicia um descanso para algumas funções do drive
#define STP 7 // Avanço do passo
#define DIR 6 // Direção do passo
//---- Variáveis de controle ----
int PPR = 0;      // pulsos por resolução
int passo = 0;    // passos
int temp = 1000;  // tempo entre os passos 
void setup()
{
  pinMode(ENA, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(STP, OUTPUT);
  pinMode(DIR, OUTPUT);
  digitalWrite(SLP, LOW);   // Sleep desabilitado 
  digitalWrite(DIR, HIGH);  // Sentido Horário habilitado
  digitalWrite(RST, HIGH);  // RST habilitado 
  Serial.begin(9600);
  Serial.println("Pressione: r- para resetar // s - para iniciar Sleep // o - para desativar Sleep");
  delay(100);
  Serial.println("1 para FULL - STEP // 2 para HALF - STEP // 3 para Quarter - STEP // 4 para Eighth - STEP // 5 para Sixteenth - STEP");
}

void loop()
{
  char choice = Serial.read(); // Lê os caracteres escritos no monitor serial

  if (choice == '1')
  {                           // Se receber 1
    Serial.println("Full");   
    full();                  // Inicia o void full
    volta();                 // Inicia o void volta 
  }
  if (choice == '2')
  {                           // Se receber 2
    Serial.println("Half");
    half();                   // Inicia o void half
    volta();
  }
  if (choice == '3')
  {                           // Se receber 3
    Serial.println("1 / 4");
    quarter();                // Inicia o void quarter
    volta();
  }
  if (choice == '4')
  {                          // Se receber 4
    Serial.println("1 / 8");
    eighth();                // Inicia o void eighth
    volta();
  }
  if (choice == '5')
  {                          // Se receber 5
    Serial.println("1 / 16");
    sixteenth();             // Inicia o void sixteenth
    volta();
  }
  if (choice == 'r')
  {                           // Se receber r
    Serial.println("Iniciar Reset");
    rst();                    // Inicia o void rst
  }
  if (choice == 's')
  {                           // Se receber s
    Serial.println("Iniciar Sleep");
    Sleepon();                // Inicia o void Sleepon
  }
  if (choice == 'o')
  {                            // Se receber o
    Serial.println("Desligar Sleep");
    Sleepoff();               // Inicia o void Sleepoff
  }

}
void full()
{
  PPR = 200;               // Em Full - 200 passos
  Serial.println("Modo Passo Completo (Full - Step)");
  digitalWrite(MS1, LOW);  
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
}
void half()
{
  PPR = 400;               // Em half - 400 passos
  Serial.println("Modo Meio Passo(Half - Step)");
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
}
void quarter()
{
  PPR = 800;               // Em quarter - 800 passos
  Serial.println("Modo Quarter - STEP (1 / 4)");
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, LOW);
}
void eighth()
{
  PPR = 1600;             // Em eighth - 1600 passos
  Serial.println("Modo Eighth - STEP (1 / 8)");
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, LOW);
}
void sixteenth()
{                      // Em sixteenth - 3200 passos
  PPR = 3200;
  Serial.println("Modo Sixteenth - STEP (1 / 16)");
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);
}
void volta() {
  HR();  // Inicia o sentido horário
  while (PPR > passo)
  {      // Enquanto PPR for maior que passo
     // Avança o passo
    digitalWrite(STP, LOW);
    delayMicroseconds(temp); //Tempo em Microseconds
    digitalWrite(STP, HIGH);
    delayMicroseconds(temp);
    passo++; // Aumenta em 1 o valor de passo
  }
  passo = 0; // valor de passso muda pra 0 
  ena2();    
  delay(1000);
  ena();
  AH();      // Inicia o Sentido Anti-horário
  while (PPR > passo) 
  {      // Enquanto PPR for maior que passo
         // Avança o passo
    digitalWrite(STP, LOW);
    delayMicroseconds(temp);
    digitalWrite(STP, HIGH);
    delayMicroseconds(temp);
    passo++;
  }
  passo = 0;
  ena2();
  delay(1000);
  ena();

}
void ena2() {
  digitalWrite(ENA, HIGH);  // Ativa o A4988
  delay(10);
}
void ena() {
  digitalWrite(ENA, LOW);   // Desativa o A4988
  delay(10);
}
void HR() {                // Sentido Horário
  passo = 0;     
  Serial.println("Sentido - Horario");
  digitalWrite(DIR, HIGH);
}
void AH()  {               // Sentido Anti-Horário 
  passo = 0;
  Serial.println("Sentido - Anti-horario");
  digitalWrite(DIR, LOW);
}
void rst()  {      // Reseta os drivers 
  digitalWrite(RST, LOW);
  delay(10);
  digitalWrite(RST, HIGH);
  delay(10);
  Serial.println("Reset OK");
}
void Sleepoff()  {   // Desativa o modo Sleep
  Serial.println("Sleep off");
  digitalWrite(SLP, LOW);
  delay(10);
}
void Sleepon()  {   // Ativa o modo Sleep
  Serial.println("Sleep on");
  digitalWrite(SLP, HIGH);
  delay(10);
}
