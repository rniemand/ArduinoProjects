#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.clearBuffer();         // clear the internal memory
  //u8g2.setFont(u8g2_font_u8glib_4_tf); // choose a suitable font
  //u8g2.setFont(u8g2_font_ncenB14_tr); // choose a suitable font
  //u8g2.setFont(u8g2_font_ncenB14_tf); // choose a suitable font
  //u8g2.setFont(u8g2_font_u8glib_4_hf); // choose a suitable font
  //u8g2.setFont(u8g2_font_baby_tf); // choose a suitable font
  u8g2.setFont(u8g2_font_courR08_tf); // choose a suitable font
  u8g2.drawStr(0,8,"IP: 192.168.0.5");  // write something to the internal memory
  u8g2.drawStr(0,16,"AP: PickMe");  // write something to the internal memory
  u8g2.drawStr(0,24,"Status: Connecting");  // write something to the internal memory
  u8g2.drawStr(0,32,"Hello World! 1 2 3 ...");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000);  
}

