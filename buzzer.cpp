
// Librerias
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#define PIN_BUZZER 5
// Definiciones de las frecuencias para la escala de Do5 a Do6
#define DO6 1046.50
#define DO6s 1108.73
#define RE6 1174.66
#define RE6s 1244.51
#define MI6 1318.51
#define FA6 1396.91
#define FA6s 1479.98
#define SOL6 1567.98
#define SOL6s 1661.22
#define LA6 1760.00
#define LA6s 1864.66
#define SI6 1975.53

#define DO7 2093.00
#define DO7s 2217.46
#define RE7 2349.32
#define RE7s 2489.02
#define MI7 2637.02
#define FA7 2793.83
#define FA7s 2959.96
#define SOL7 3135.96
#define SOL7s 3322.44
#define LA7 3520.00
#define LA7s 3729.31
#define SI7 3951.07

#define DO8 4186.01

const float notasDo6aDo8[] = {DO6, DO6s, RE6, RE6s, MI6, FA6, FA6s, SOL6, SOL6s, LA6, LA6s, SI6,
                              DO7, DO7s, RE7, RE7s, MI7, FA7, FA7s, SOL7, SOL7s, LA7, LA7s, SI7, DO8};

const String nombresNotasDo6aDo8[] = {"Do6", "Do6#", "Re6", "Re6#", "Mi6", "Fa6", "Fa6#", "Sol6", "Sol6#", "La6", "La6#", "Si6",
                                      "Do7", "Do7#", "Re7", "Re7#", "Mi7", "Fa7", "Fa7#", "Sol7", "Sol7#", "La7", "La7#", "Si7", "Do8"};
void setup()
{
    pinMode(5, OUTPUT); // Configurar el pin 5 como salida
    Serial.begin(9600); // Iniciar comunicación serial a 9600 bps
}

void loop()
{
    // for (int i = 0; i < 12; i++) {
    //   // Reproducir la nota en la escala de Do6
    //   tone(5, notasDo6[i], 1000); // Duración: 500 ms
    //   Serial.println(nombresNotasDo6[i]); // Imprimir el nombre de la nota en el monitor serial
    //   delay(1000); // Esperar 500 ms
    //   noTone(5);
    //   delay(1000);
    // }
    tone(5, SOL6, 1000);
    delay(150);
    tone(5, SI6, 1000);
    delay(150);
    tone(5, SOL7, 1000);
    delay(150);
    tone(5, MI7, 1000);
    delay(150);
    tone(5, FA7, 1000);
    delay(150);
    tone(5, SI7, 150);
    delay(50000);
}