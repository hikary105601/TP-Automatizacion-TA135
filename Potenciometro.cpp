#include <Arduino.h>
#include "Potenciometro.h"
#include "MisConstantes.h"


float angulo_pote(int lectura_pote) {
  return lectura_pote * (270.0 / 1023.0);
}

unsigned long tiempo_lectura_pote(){ // microsegundos
  unsigned long antes_pote = micros();
  int valor_pote = analogRead(PIN_POTE);
  unsigned long despues_pote = micros();
  return despues_pote - antes_pote;
}
