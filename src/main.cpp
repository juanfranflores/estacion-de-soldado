
//Librerias
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>


//Definición de pines
#define HEATER 10
#define TEMP_SENSOR A0
#define ENCODER_CANAL_A 2
#define ENCODER_CANAL_B 3
#define PUSHBUTTON 4

//Variables de temperatura
int tempSet = 0;
int tempNow = 0;
String tempSetString = "";
String tempNowString = "";
int tempSetPrev = 0;
int tempNowPrev = 0;
int deltaTemp = 5;
const int maxTemp = 400;
const int minTemp = 0;

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

//Inicializo el display
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

void setup(void) 
{
  pinMode(ENCODER_CANAL_A, INPUT);
  pinMode(ENCODER_CANAL_B, INPUT);
  u8g2.begin();
  u8g2.enableUTF8Print();
  updateDisplay(0,0);
  attachInterrupt(digitalPinToInterrupt(ENCODER_CANAL_A), giroEncoder, RISING);
}

void loop(void)
{
  if (tempSet != tempSetPrev || tempNow != tempNowPrev)
  {
    if (tempSet <minTemp){
      tempSet = minTemp;
    }
    if (tempSet>maxTemp){
      tempSet = maxTemp;
    }
    tempSetPrev = tempSet;
    tempNowPrev = tempNow;
    updateDisplay(tempSet,tempNow);
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

void updateDisplay(int temp1, int temp2){
  u8g2.firstPage(); //TODO esto tiene que estar sí o sí?
  do
  {
    u8g2.setFont(u8g2_font_spleen16x32_mf);
    // Set temperature
    u8g2.setCursor(1, 30);
    u8g2.print(F("T :    °"));
    u8g2.setCursor(1 + 16 * 4, 30);
    u8g2.print(tempToSting(temp1));

    // Now temperature
    u8g2.setCursor(1, 62);
    u8g2.print(F("T :    °"));
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

void giroEncoder()
{
  noInterrupts();
  lecturaA = digitalRead(ENCODER_CANAL_A);
  lecturaB = digitalRead(ENCODER_CANAL_B);
  izquierda = lecturaA ^ lecturaB;
  ultimaLecturaA = lecturaA;
  ultimaLecturaB = lecturaB;
  tempSet += (!izquierda-izquierda) * deltaTemp;
  interrupts();
}