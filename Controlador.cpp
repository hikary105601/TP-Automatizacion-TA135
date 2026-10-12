#include "Controlador.h"
#include "MisConstantes.h"

float control_bilinear(float* e, float* u){ //entra error (e), sale servo (u)
  return 11.44*e[0] - 18.35*e[1] + 7.356*e[2] + 0.6667*u[1]+0.3333*u[2];
}

float control_p(float* e){ // error de distancia en instante k
  float k_p = 5.0; // oscila con 10
  return k_p * e[0];
}

float control_pi(float* integral_acumulado, float* e){
  float k_i = 0.0000026;
  float I_k = *integral_acumulado + (MICROS_50HZ/2) * (e[0] + e[1]); //I_k = I_{k-1} + T/2 * e_k + T/2 * e_{k-1}
  *integral_acumulado = I_k;
  
  float p =  control_p(e);
  return p + k_i * I_k;
}

float control_pid(float* derivativo_acumulado, float* integral_acumulado, float* e){
  float k_d = 0.9;
  float D_k1;
  float D_k = (2.0/MICROS_50HZ) * (e[0] - e[1]) - *derivativo_acumulado;
  *derivativo_acumulado = D_k;

  float pi = control_pi(integral_acumulado, e);
  return pi + k_d * D_k;
}