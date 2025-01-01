#include <Servo.h>
Servo servo1; 
Servo servo2;
#include <Wire.h>

// Doit être ajusté en fonction de chaque nunchuck
#define ZEROX 530  
#define ZEROY 530
#define ZEROZ 530

#define TRIM 20

// adresse I2C du nunchuck
#define WII_NUNCHUK_I2C_ADDRESS 0x52


// définition d'une variable counter
int counter;

// définition d'un tableau de données
uint8_t data[6];

void setup() 
{ 
  // on attache le servomoteur à la pin 11 (PWM)
  servo1.attach(11);
  servo2.attach(12);

  // initialisation du nunchuck
  Wire.begin();

  Wire.beginTransmission(WII_NUNCHUK_I2C_ADDRESS);
  Wire.write(0xF0);
  Wire.write(0x55);
  Wire.endTransmission();

  Wire.beginTransmission(WII_NUNCHUK_I2C_ADDRESS);
  Wire.write(0xFB);
  Wire.write(0x00);
  Wire.endTransmission();
} 

void loop() 
{ 
    // on demande 6 octets au nunchuck
    Wire.requestFrom(WII_NUNCHUK_I2C_ADDRESS, 6);

    counter = 0;
    // tant qu'il y a des données
    while(Wire.available())
    {
      // on récupère les données
      data[counter++] = Wire.read();
    }

    // on réinitialise le nunchuck pour la prochaine demande
    Wire.beginTransmission(WII_NUNCHUK_I2C_ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();

    if(counter >= 5)
    {
      // on extrait les données
      double accelX = ((data[2] << 2) + ((data[5] >> 2) & 0x03) - ZEROX);
      double accelY = ((data[3] << 2) + ((data[5] >> 4) & 0x03) - ZEROY);
      double accelZ = ((data[4] << 2) + ((data[5] >> 6) & 0x03) - ZEROZ);

      // données d'accélération de l'axe Y
      // on limite la valeur entre -180 et 180
      int value = constrain(accelY, -180, 180);
      // on mappe cette valeur pour le servomoteur soit entre 0 et 180
      value = map(value, -180, 180, 0+TRIM, 180-TRIM);
      // on écrit sur le servomoteur la valeur
      servo1.write((180-TRIM)-value);
      
      // données d'accélération de l'axe X
      value = constrain(accelX, -180, 180);
      value = map(value, -180, 180, 0+TRIM, 180-TRIM); 
      servo2.write((180-TRIM)-value);

      // un petit delai pour pas saturer le servomoteur
      delay(40);
    }
}
