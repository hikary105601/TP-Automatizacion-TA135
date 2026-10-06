#ifndef MiSonar_h
#define MiSonar_h

#include <NewPing.h>

float distancia_cm(NewPing &sonar);
unsigned long tiempo_lectura_sonar(NewPing &sonar);

#endif