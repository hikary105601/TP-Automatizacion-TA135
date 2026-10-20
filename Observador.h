#ifndef Observador_h
#define Observador_h

enum variables_observador{
  angEst,      // angulo de barra estimado
  angReal,     // angulo imu
  velEst,      // velocidad angular estimada
  velReal      // velocidad angular giroscopo
};

float* estimador_variables(float* datos_observador, float salida_real);

#endif