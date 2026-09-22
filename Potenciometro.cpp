#include <Arduino.h>
#include "Potenciometro.h"
#include "MisConstantes.h"


float angulo_pote(int lectura_pote) {
  return lectura_pote * (270.0 / 1023.0);
}

void tiempo_lectura_pote(){
  unsigned long antes_pote = micros();
  volatile int valor_pote = analogRead(PIN_POTE);
  unsigned long despues_pote = micros();
  Serial.print("Tiempo de lectura de pote: ");
  Serial.println(despues_pote - antes_pote);
}
