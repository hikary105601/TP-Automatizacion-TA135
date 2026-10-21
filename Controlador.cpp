#include "Controlador.h"
#include "MisConstantes.h"

float control_bilinear(float* e, float* u){ //entra error (e), sale servo (u)
  return 11.44*e[0] - 18.35*e[1] + 7.356*e[2] + 0.6667*u[1]+0.3333*u[2];
}

float control_p(float* e){ // error de distancia en instante k
  float k_p = 3.8;
  return k_p * e[0];
}

float control_d(float* derivativo_acumulado, float* e){
  float k_d = 0.4;
  float D_k = (2.0/MICROS_50HZ) * (e[0] - e[1]) - *derivativo_acumulado;
  *derivativo_acumulado = D_k;
  return k_d * D_k;
}
float control_i(float* integral_acumulado, float* e){
  float k_i = 0.00000002;
  float I_k = *integral_acumulado + (MICROS_50HZ/2) * (e[0] + e[1]); //I_k = I_{k-1} + T/2 * e_k + T/2 * e_{k-1}
  *integral_acumulado = I_k;
  return k_i * I_k;
}

float control_pd(float* derivativo_acumulado, float* e){
  float p = control_p(e);
  float d = control_d(derivativo_acumulado,e);
  return p + d;
}

float control_pi(float* integral_acumulado, float* e){
  float p =  control_p(e);
  float i = control_i(integral_acumulado,e);
  return p + i;
}

float control_pid(float* derivativo_acumulado, float* integral_acumulado, float* e){
  float pi = control_pi(integral_acumulado, e);
  float pd = control_pd(derivativo_acumulado, e);
  // pi + pd = p + k_i * I_k + p + k_d * D_k = 2p + k_i * I_k
  return pi;
}