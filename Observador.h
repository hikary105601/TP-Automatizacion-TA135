#ifndef Observador_h
#define Observador_h

enum variables_observador{
  angEst,      // angulo de barra estimado
  angReal,     // angulo imu
  velEst,      // velocidad angular estimada
  velReal      // velocidad angular giroscopo
};

void actualizar_estimadas(float* datos_observador);

#endif