#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>


#define HEATER 10
#define SENSOR A0

int tPrevioDisplay;
int tPrevioTemp;

//tSet
int tSet = 0;
String tSetString = "";
int tSetPrev = 0;

//tNow
int tNow = 0;
String tNowString = "";
int tNowPrev = 0;

//Funciones
String tempToSting(int);
void updateDisplay();

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

void setup(void)
{
  u8g2.begin();
  u8g2.enableUTF8Print();
}

void loop(void)
{
  if (tSet != tSetPrev || tNow != tNowPrev)
  {
    tSetPrev = tSet;
    tSetString = tempToSting(tSet);
    tNowPrev = tNow;
    tNowString = tempToSting(tNow);
  }

  if (millis() - tPrevioTemp > 200)
  {
    tNow += 1;
    tPrevioTemp = millis();
  }

  if (millis() - tPrevioDisplay > 100)
  {
    tPrevioDisplay = millis();
    u8g2.firstPage();
    updateDisplay();
  }
  }

String tempToSting(int temp){
  if (temp>=0 && temp<10){
    return ("  "+String(temp));
  }
  if (temp>=10 && temp<100)
  {
    return (" " + String(temp));
  }
  return String(temp);
}

void updateDisplay(){
  do
  {
    u8g2.setFont(u8g2_font_spleen16x32_mf);
    // Set temperature
    u8g2.setCursor(1, 30);
    u8g2.print(F("T :    °"));
    u8g2.setCursor(1 + 16 * 4, 30);
    u8g2.print(tSetString);

    // Now temperature
    u8g2.setCursor(1, 62);
    u8g2.print(F("T :    °"));
    u8g2.setCursor(1 + 16 * 4, 62);
    u8g2.print(tNowString);

    // Letras chicas
    u8g2.setFont(u8g2_font_spleen5x8_mf);
    u8g2.setCursor(1 + 16, 30);
    u8g2.print(F("set"));
    u8g2.setCursor(1 + 16, 62);
    u8g2.print(F("now"));
  } while (u8g2.nextPage());
}
