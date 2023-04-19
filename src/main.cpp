
// Librerias
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "OneButton.h"

// Definición de pines
#define HEATER 10
#define TEMP_SENSOR A0
#define ENCODER_CANAL_A 3
#define ENCODER_CANAL_B 4
#define PUSHBUTTON 5
#define BUZZERPIN 2
#define MINUTO 60000
const unsigned long t10 = MINUTO * 10; // Diez minutos
const unsigned long t2 = MINUTO * 2;   // Dos minutos
bool poweroffAlarm = false;
bool tempReachedAlarm = false;
unsigned long lastMillisPoweroff = 0;
// Variables de temperatura
int tempSet = 0;
int tempNow = 217;
int tempSetPrev = 0;
int tempNowPrev = 0;

int deltaTemp = 5;
const int maxTemp = 400;
const int minTemp = 0;
const int preset1 = 315;
const int preset2 = 340;
const int preset3 = 370;
const int tempTermocontraible = 100;
int lastPreset = 0;

// Variables para el estado del encoder
volatile bool ultimaLecturaA = LOW;
volatile bool ultimaLecturaB = LOW;
volatile bool lecturaA = LOW;
volatile bool lecturaB = LOW;
volatile bool izquierda = false;

// Funciones
String tempToSting(int);
void updateDisplay(int temp1, int temp2);
void giroEncoder();
void singleClick();
void doubleClick();
void longPress();

// Inicializo el display
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE);
OneButton button(PUSHBUTTON, true);

void setup(void)
{
  pinMode(ENCODER_CANAL_A, INPUT);
  pinMode(ENCODER_CANAL_B, INPUT);
  pinMode(PUSHBUTTON, INPUT_PULLUP);
  pinMode(BUZZERPIN, OUTPUT);
  u8g2.begin();
  u8g2.enableUTF8Print();
  updateDisplay(0, 0);
  attachInterrupt(digitalPinToInterrupt(ENCODER_CANAL_A), giroEncoder, RISING);
  button.attachClick(singleClick);
  button.attachDoubleClick(doubleClick);
  button.attachLongPressStart(longPress);
}

void loop(void)
{
  // TODO luego de leer la temperatura actual y actuar sobre el soldador, tengo que actualizar el display


  // poweroff
  if (millis() - lastMillisPoweroff > t10 && tempSet != minTemp)
  {
    if (!poweroffAlarm)
    {
      tone(5, 2489.02, 150);
      delay(150);
      tone(5, 1864.66, 150);
      delay(150);
      tone(5, 2489.02, 150);
      delay(150);
      tone(5, 1864.66, 150);
      delay(150);
      poweroffAlarm = true;
    }
    else
    {
      if (millis() - lastMillisPoweroff > t2)
      {
        tone(5, 2489.02, 150);
        delay(150);
        tone(5, 1864.66, 150);
        delay(150);
        tone(5, 2489.02, 150);
        delay(150);
        tone(5, 1864.66, 150);
        delay(1000);
        tone(5, 2489.02, 150);
        delay(150);
        tone(5, 1864.66, 150);
        delay(150);
        tone(5, 2489.02, 150);
        delay(150);
        tone(5, 1864.66, 150);
        delay(150);
        tempSet = minTemp;
        poweroffAlarm = false;
      }
    }
  }
  // Leo si hubo cambios en el pulsador
  button.tick();
  // Si hubo cambios de temperatura actualizo el display y actuo sobre el soldador.
  if (tempSet != tempSetPrev)
  {
    tempReachedAlarm = false;

    tempSetPrev = tempSet;
    tempNowPrev = tempNow;
    updateDisplay(tempSet, tempNow);
  }
  if (tempNow == tempSet && tempSet != 0 && !tempReachedAlarm)
  {
    tempReachedAlarm = true;
    tone(BUZZERPIN, 1567.98, 1000);
    delay(150);
    tone(BUZZERPIN, 1975.53, 1000);
    delay(150);
    tone(BUZZERPIN, 3135.96, 1000);
    delay(150);
    tone(BUZZERPIN, 2637.02, 1000);
    delay(150);
    tone(BUZZERPIN, 2793.83, 1000);
    delay(150);
    tone(BUZZERPIN, 3951.07, 150);
  }
}

String tempToSting(int temp)
{
  if (temp >= 0 && temp < 10)
  {
    return ("  " + String(temp) + "°");
  }
  if (temp >= 10 && temp < 100)
  {
    return (" " + String(temp) + "°");
  }
  return (String(temp) + "°");
}

void updateDisplay(int temp1, int temp2)
{
  u8g2.firstPage(); // TODO esto tiene que estar sí o sí?
  do
  {
    u8g2.setFont(u8g2_font_spleen16x32_mf);
    // Set temperature
    u8g2.setCursor(1, 30);
    u8g2.print(F("T :"));
    u8g2.setCursor(1 + 16 * 4, 30);
    if (temp1 == 0)
    {
      u8g2.print(" OFF");
    }
    else
    {
      u8g2.print(tempToSting(temp1));
    }

    // Now temperature
    u8g2.setCursor(1, 62);
    u8g2.print(F("T :"));
    u8g2.setCursor(1 + 16 * 4, 62);
    u8g2.print(tempToSting(temp2));

    // Letras chicas
    u8g2.setFont(u8g2_font_spleen5x8_mf);
    u8g2.setCursor(1 + 16, 30);
    u8g2.print(F("set"));
    u8g2.setCursor(1 + 16, 62);
    u8g2.print(F("now"));
  } while (u8g2.nextPage());
}

// Funciones del encoder con pulsador
void giroEncoder()
{
  lastMillisPoweroff = millis();
  noInterrupts();
  lecturaA = digitalRead(ENCODER_CANAL_A);
  lecturaB = digitalRead(ENCODER_CANAL_B);
  izquierda = lecturaA ^ lecturaB;
  ultimaLecturaA = lecturaA;
  ultimaLecturaB = lecturaB;
  tempSet += (!izquierda - izquierda) * deltaTemp;
  if (tempSet < minTemp)
  {
    tempSet = minTemp;
  }
  if (tempSet > maxTemp)
  {
    tempSet = maxTemp;
  }
  interrupts();
}

void singleClick()
{
  lastMillisPoweroff = millis();
  if (lastPreset == minTemp || lastPreset == tempTermocontraible)
  {
    tempSet = preset1;
    lastPreset = preset1;
  }
  else if (lastPreset == preset1)
  {
    tempSet = preset2;
    lastPreset = preset2;
  }
  else if (lastPreset == preset2)
  {
    tempSet = preset3;
    lastPreset = preset3;
  }
  else if (lastPreset == preset3)
  {
    tempSet = maxTemp;
    lastPreset = maxTemp;
  }
  else if (lastPreset == maxTemp)
  {
    tempSet = tempTermocontraible;
    lastPreset = tempTermocontraible;
  }
}

void doubleClick()
{
  lastMillisPoweroff = millis();
  if (tempSet != minTemp)
  {
    tempSet = minTemp;
    lastPreset = minTemp;
  }
  else
  {
    tempSet = preset2;
    lastPreset = preset2;
  }
}

void longPress()
{
  if (deltaTemp == 5)
  {
    deltaTemp = 1;
  }
  else
  {
    deltaTemp = 5;
  }
}