#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Construtor para display OLED 128x32 via I2C
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

int contador = 0;

void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.clearBuffer();

  // Fonte grande e sem serifa, ideal para números
  u8g2.setFont(u8g2_font_logisoso32_tn);  // Fonte de números grandes (32px de altura)

  // Converte número para string
  char texto[10];
  sprintf(texto, "%d", contador);

  // Centraliza horizontalmente
  int largura = u8g2.getStrWidth(texto);
  int x = (128 - largura) / 2;

  u8g2.drawStr(x, 31, texto);  // 31 é a base da linha para displays 32px de altura
  u8g2.sendBuffer();

  contador++;
  delay(100);  // 1 segundo
}