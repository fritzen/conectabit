// DS1302:  RST pin    -> Arduino Digital 4
//          DATA pin   -> Arduino Digital 3
//          CLK pin  -> Arduino Digital 2
#include <U8glib.h>
#include "DS1302.h"
#include <Wire.h>  

#define BTN_ADJUST 8
#define BTN_PLUS 9
#define BTN_MINUS 10

DS1302 rtc(4, 3, 2);

/* ATENÇÃO: CASO O DISPLAY FALHE NA INICIALIZAÇAO (NÃO EXIBA CORRETAMENTE OS CARACTERES),
   DEVE-SE REMOVER E RECONECTAR A ALIMENTAÇÃO */

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 

byte adjust_step = 0; //1=hh, 2=mm, 3=ss, 4=dow, 5=dd, 6=mm, 7=yyyy, 
bool focus = false;

  uint8_t   hour;
  uint8_t   min;
  uint8_t   sec;
  uint8_t   date;
  uint8_t   mon;
  uint16_t  year;
  uint8_t   dow;
  
void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  //u8g.setFont(u8g_font_unifont);
  //https://github.com/olikraus/u8glib/tree/master/fntsrc
  
  //u8g.setFont(u8g_font_unifont_78_79); // u8g_font_9x15 ou u8g_font_fub17 ou u8g_font_9x18B (desenhos u8g_font_unifont_77)
  //u8g.print("1");  
  u8g.setPrintPos(0, 20);
  
  
  u8g.setFont(u8g_font_9x15);
  u8g.print(" ");
  if (adjust_step == 1) {
    if (focus) {
        u8g.print(GetFormated(hour));
     } else {
        u8g.print("  ");  
     }
  } else {
    u8g.print(GetFormated(hour));
  }

u8g.print(":");

  if (adjust_step == 2) {
    if (focus) {
        u8g.print(GetFormated(min));
     } else {
        u8g.print("  ");  
     }
  } else {
    u8g.print(GetFormated(min));
  }

u8g.print(":");

  if (adjust_step == 3) {
    if (focus) {
        u8g.print(GetFormated(sec));
     } else {
        u8g.print("  ");  
     }
  } else {
    u8g.print(GetFormated(sec));
  }

  //u8g.print(" ");
  //u8g.setFont(u8g_font_unifont_78_79); // u8g_font_9x15 ou u8g_font_fub17 ou u8g_font_9x18B (desenhos u8g_font_unifont_77)
  //u8g.print("1");  
  

  u8g.setFont(u8g_font_5x7);
  u8g.setPrintPos(0, 32); 

  if (adjust_step == 4) {
    if (focus) {
        u8g.print(rtc.getDOWStr(FORMAT_SHORT));
     } else {
        u8g.print("         ");  
     }
  } else {
    u8g.print(rtc.getDOWStr(FORMAT_SHORT));
  }
  

  u8g.print(", ");


  
  if (adjust_step == 5) {
    if (focus) {
        u8g.print(GetFormated(date));
     } else {
        u8g.print("__");  
     }
  } else {
    u8g.print(GetFormated(date));
  }

  u8g.print("/");

  if (adjust_step == 6) {
    if (focus) {
        u8g.print(GetFormated(mon));
     } else {
        u8g.print("__");  
     }
  } else {
    u8g.print(GetFormated(mon));
  }

  u8g.print("/");

  if (adjust_step == 7) {
    if (focus) {
        u8g.print(GetFormated(year));
     } else {
        u8g.print("____");  
     }
  } else {
    u8g.print(GetFormated(year));
  }

}


char* GetFormated(int value){
  static char tmp[2];
  sprintf(tmp, "%02d", value);
  return tmp;
}


void setup()
{ 
  // Set the clock to run-mode, and disable the write protection
  rtc.halt(false);
  rtc.writeProtect(false);
  
  Serial.begin(9600);

  pinMode(BTN_ADJUST, INPUT_PULLUP);
  pinMode(BTN_PLUS, INPUT_PULLUP);
  pinMode(BTN_MINUS, INPUT_PULLUP);
  pinMode(13, OUTPUT);


 // assign default color value


  // CASO QUEIRA REALIZAR O AJUSTE VIA CÓDIGO
  //adjustDateTime();
  
}

void splash(void) {

  u8g.firstPage();

    do {
        // graphic commands to redraw the complete screen should be placed here  
      u8g.setFont(u8g_font_unifont);
      u8g.drawStr( 0, 22, "Relogio");
  } while( u8g.nextPage() );

  

}

void adjustDateTime(){
    // The following lines can be commented out to use the values already stored in the DS1302
    rtc.setDOW(WEDNESDAY);        // Set Day-of-Week to WEDNESDAY
    rtc.setTime(16, 7, 0);     // Set the time to 24hr format
    rtc.setDate(27, 1, 2016);   // Set the date
  }


void clearOLED(){
    u8g.firstPage();  
    do {
    } while( u8g.nextPage() );
}

void loop()
{
  focus = !focus;

  byte sensorVal = digitalRead(BTN_ADJUST);


  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  if (sensorVal == HIGH) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
    adjust_step++;

    if (adjust_step > 7){
        adjust_step = 0;
    }
  }
  byte plus = digitalRead(BTN_PLUS);
  byte minus = digitalRead(BTN_MINUS);
  byte operation = 0;
  if (plus == LOW) {
    operation = 1;
  }
  if (minus == LOW) {
    operation = -1;
  }


  hour = rtc.getTime().hour;
  min = rtc.getTime().min;
  sec = rtc.getTime().sec;
  date = rtc.getTime().date;
  mon = rtc.getTime().mon;
  year = rtc.getTime().year;
  dow = rtc.getTime().dow;

  if (adjust_step == 1){
      rtc.setTime(hour + operation, min, sec);
  }

  if (adjust_step == 2){
      rtc.setTime(hour, min + operation, sec);
  }

  if (adjust_step == 3){
      rtc.setTime(hour, min, sec + operation);
  }

  if (adjust_step == 4){
      rtc.setDOW(dow + operation);
  }

  if (adjust_step == 5){
      rtc.setDate(date  + operation, mon, year);
  }

  if (adjust_step == 6){
      rtc.setDate(date, mon + operation, year);
  }
  
  if (adjust_step == 7){
      rtc.setDate(date, mon, year + operation);
  }

  
  //clearOLED();

    u8g.firstPage();
  do {
  //  splash(); 
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(100);

}
