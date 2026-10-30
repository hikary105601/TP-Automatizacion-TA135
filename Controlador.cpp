#include "Controlador.h"
#include "MisConstantes.h"

float control_bilinear(float* e, float* u){ //entra error (e), sale servo (u)
  return 11.44*e[0] - 18.35*e[1] + 7.356*e[2] + 0.6667*u[1]+0.3333*u[2];
}

float control_p(float e0){ // error de distancia en instante k
  float k_p = 5;
  return k_p * e0;
}

float control_d(float* derivativo_acumulado, float e0, float e1){
  float k_d = 0.4;
  float D_k = (2.0/MICROS_50HZ) * (e0 - e1) - *derivativo_acumulado; // D_k = (2/T)*(e_k-e_{k-1}) - D_{k-1} 
  *derivativo_acumulado = D_k;
  return k_d * D_k;
}

float control_i(float* integral_acumulado, float e0, float e1){
  float k_i = 0.00000002;
  float I_k = *integral_acumulado + (MICROS_50HZ/2) * (e0 + e1); //I_k = I_{k-1} + T/2 * e_k + T/2 * e_{k-1}
  *integral_acumulado = I_k;
  return k_i * I_k;
}

float control_pd(float* derivativo_acumulado, float e0, float e1){
  float p = control_p(e0);
  float d = control_d(derivativo_acumulado, e0, e1);
  return p + d;
}

float control_pi(float* integral_acumulado, float e0, float e1){
  float p =  control_p(e0);
  float i = control_i(integral_acumulado,e0, e1);
  return p + i;
}

float control_pid(float* derivativo_acumulado, float* integral_acumulado, float e0, float e1){
  float p = control_p(e0);
  float pi = control_pi(integral_acumulado, e0, e1);
  float pd = control_pd(derivativo_acumulado, e0, e1);
  // pi + pd = p + k_i * I_k + p + k_d * D_k = 2p + k_i * I_k
  return pi + pd - p;
}