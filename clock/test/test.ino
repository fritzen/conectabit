// DS1302:  RST pin    -> Arduino Digital 4
//          DATA pin   -> Arduino Digital 3
//          CLK pin  -> Arduino Digital 2
#include <U8glib.h>
#include <Wire.h>  


U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 

  


void setup()
{ 

  Serial.begin(9600);


}

void splash(void) {

  u8g.firstPage();

    do {
        // graphic commands to redraw the complete screen should be placed here  
      u8g.setFont(u8g_font_unifont);
      u8g.drawStr( 0, 22, "Relogio");
  } while( u8g.nextPage() );

}

void loop()
{

  
  //clearOLED();

    splash(); 

  // rebuild the picture after some delay
  delay(100);

}
