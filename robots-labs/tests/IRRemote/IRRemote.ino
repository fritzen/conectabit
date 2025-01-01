#include <IRremote.h>
int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  irrecv.enableIRIn();
  
  Serial.println("Enabled IRin");
  //UP - FF629D
  //LEFT - FF22DD
  //RIGHT - FFC23D
  //DOWN - FFA857
  //OK - FF02FD
  //1 - FF6897
  //2 - FF9867
  //3 - FFB04F
  //4 - FF30CF
  //5 - FF18E7
  //6 - FF7A85
  //7 - FF10EF
  //8 - FF38C7
  //9 - FF5AA5
  //0 - FF4AB5
  //* - FF42BD
  //# - FF52AD
}
void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    Serial.println("-");
    irrecv.resume();
  }
  delay(100);
  Serial.println("-");
}  
