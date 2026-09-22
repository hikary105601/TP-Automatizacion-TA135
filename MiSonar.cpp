#include <Arduino.h>
#include "MiSonar.h"
#include "MisConstantes.h"

float distancia_cm(NewPing &sonar){
  unsigned int uS = sonar.ping();  // Tiempo de vuelo ida y vuelta
  return uS / VEL_SONIDO;
}

void tiempo_lectura_sonar(NewPing &sonar){
  unsigned long antes_sonar = micros();
  sonar.ping();
  unsigned long despues_sonar = micros();
  Serial.print("Tiempo de lectura de sonar: ");
  Serial.println(despues_sonar - antes_sonar);
}