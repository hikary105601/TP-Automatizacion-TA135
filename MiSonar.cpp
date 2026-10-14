#include <Arduino.h>
#include "MiSonar.h"
#include "MisConstantes.h"

float distancia_cm(NewPing &sonar){
  unsigned int uS = sonar.ping(DISTANCIA_MAX);  // Tiempo de vuelo ida y vuelta
  return (uS/2) / VEL_SONIDO;
}

unsigned long tiempo_lectura_sonar(NewPing &sonar){ // microsegundos
  unsigned long antes_sonar = micros();
  sonar.ping(DISTANCIA_MAX);
  unsigned long despues_sonar = micros();
  return despues_sonar - antes_sonar;
}