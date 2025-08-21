#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <Servo.h>

#include "captive_html.h"
#include "joystick_html.h"

// Definindo os servos
Servo myservo1;
Servo myservo2;

// WebSocket e servidor
WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);
DNSServer dnsServer;

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);

// Definição dos pinos do ESP-01
const int pwmMotorA = 0; // GPIO0
const int dirMotorA = 3; // RX (GPIO3)
const int pwmMotorB = 2; // GPIO2
const int dirMotorB = 1; // TX (GPIO1)

// Novas constantes para os servos com números diretamente
const int servoPin1 = 0;  // GPIO0 para servo 1
const int servoPin2 = 1;  // GPIO1 para servo 2

IPAddress local_IP(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

bool isClaw = false;
bool useServo = false; // Por padrão, usa motores normais com L9110

void setup() {
  Serial.begin(9600);
  
  // Configuração de servos
  if (useServo) {
    myservo1.attach(servoPin1); // Servo 1 no GPIO0
    myservo2.attach(servoPin2); // Servo 2 no GPIO1
  }

  // Configurando os motores
  pinMode(pwmMotorA, OUTPUT);
  pinMode(pwmMotorB, OUTPUT);
  pinMode(dirMotorA, OUTPUT);
  pinMode(dirMotorB, OUTPUT);

  // Configurando o WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("ROBOT-NETWORK");

  // Iniciando WebSocket e servidor
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  server.on("/", []() { server.send_P(200, "text/html", joystick_html); });
  server.begin();
}

void loop() {
  webSocket.loop();
  server.handleClient();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    String txt = "";
    for (int i = 0; i < length; i++) {
      txt = txt + ((char)payload[i]);
    }

    int delimiterIndex = txt.indexOf(',');

    if (delimiterIndex != -1) {
      String xStr = txt.substring(0, delimiterIndex);
      String yStr = txt.substring(delimiterIndex + 1);
      int x = xStr.toInt();
      int y = yStr.toInt();

      if (useServo) {
        controlServo(x, y);
      } else {
        controlMotor(x, y);
      }
    }
  }
}

void controlMotor(int x, int y) {
  // Controle do Motor A (Baseado no valor de x)
  if (x == 0) {
    moveMotorForwardWithSpeed(1, 0);  // Parar o Motor A
  } else if (x > 0) {
    int speedA = map(x, 0, 255, 0, 255);  // Mapeia x para velocidade positiva
    moveMotorForwardWithSpeed(1, speedA);  // Motor A para frente com velocidade mapeada
  } else {
    int speedA = map(abs(x), 0, 255, 0, 255);  // Mapeia o valor absoluto de x para velocidade
    moveMotorBackwardWithSpeed(1, speedA);  // Motor A para trás com velocidade mapeada
  }

  // Controle do Motor B (Baseado no valor de y)
  if (y == 0) {
    moveMotorForwardWithSpeed(2, 0);  // Parar o Motor B
  } else if (y > 0) {
    int speedB = map(y, 0, 255, 0, 255);  // Mapeia y para velocidade positiva
    moveMotorForwardWithSpeed(2, speedB);  // Motor B para frente com velocidade mapeada
  } else {
    int speedB = map(abs(y), 0, 255, 0, 255);  // Mapeia o valor absoluto de y para velocidade
    moveMotorBackwardWithSpeed(2, speedB);  // Motor B para trás com velocidade mapeada
  }
}


void controlServo(int x, int y) {
  // Mapeia os valores de entrada para o intervalo de ângulo típico de 0 a 180 graus
  int angleA = map(x, -255, 255, 0, 180);
  int angleB = map(y, -255, 255, 180, 0); // Inverte a direção para o segundo servo

  myservo1.write(angleA);
  myservo2.write(angleB);
}



// Método para movimentar o motor para frente com velocidade variável
void moveMotorForwardWithSpeed(int motor, int speed) {
  if (motor == 1) {
    analogWrite(dirMotorA, 0);        // dirMotorA fixo em 0 (para frente)
    analogWrite(pwmMotorA, speed);    // Velocidade controlada pelo pwmMotorA
  } else if (motor == 2) {
    analogWrite(dirMotorB, 0);        // dirMotorB fixo em 0 (para frente)
    analogWrite(pwmMotorB, speed);    // Velocidade controlada pelo pwmMotorB
  }
}

// Método para movimentar o motor para trás com velocidade variável
void moveMotorBackwardWithSpeed(int motor, int speed) {
  if (motor == 1) {
    analogWrite(dirMotorA, speed);    // Velocidade controlada pelo dirMotorA (para trás)
    analogWrite(pwmMotorA, 0);        // pwmMotorA fixo em 0 (controlando a direção)
  } else if (motor == 2) {
    analogWrite(dirMotorB, speed);    // Velocidade controlada pelo dirMotorB (para trás)
    analogWrite(pwmMotorB, 0);        // pwmMotorB fixo em 0 (controlando a direção)
  }
}