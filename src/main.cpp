#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Encoder.h>

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

// Variables para el estado del encoder
volatile int contador = 0;
volatile bool ultimaLecturaA = LOW;
volatile bool ultimaLecturaB = LOW;
volatile bool interrupcionDisparada = false; 

//Funciones
String tempToSting(int);
void updateDisplay(int temp1, int temp2);
void isrA();

//Declaración de módulos
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
Encoder myEnc(ENCODER_CANAL_A, ENCODER_CANAL_B);



void setup(void)
{
  u8g2.begin();
  u8g2.enableUTF8Print();
  updateDisplay(0,0);
  attachInterrupt(digitalPinToInterrupt(ENCODER_CANAL_A), isrA, CHANGE);
}

void loop(void)
{
  if (tempSet != tempSetPrev || tempNow != tempNowPrev)
  {
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

void isrA()
{
  int lecturaA = digitalRead(ENCODER_CANAL_A);
  int lecturaB = digitalRead(ENCODER_CANAL_B);

  // Calcular la dirección del giro del encoder basado en los estados actuales y anteriores
  int direccion = 0;
  if (lecturaA != ultimaLecturaA || lecturaB != ultimaLecturaB)
  {
    if (lecturaA == lecturaB)
    {
      direccion = 1; // Girar a la derecha
    }
    else
    {
      direccion = -1; // Girar a la izquierda
    }
  }

  // Actualizar tempSet en función de la dirección del giro del encoder
  tempSet += direccion * 10;
  ultimaLecturaA = lecturaA;
  ultimaLecturaB = lecturaB;
  interrupcionDisparada = false; // Bajar la bandera de interrupción
}