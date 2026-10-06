#include "Controlador.h"
#include "MisConstantes.h"

//#define MICROS_CONTROLADOR //más lento que sensores

float control_p(float e0){ // error de distancia en instante k
  float k_p = 5.0; // oscila con 10
  return k_p * e0;
}

float control_pi(float* integral_acumulado, float e0, float e1){
  float k_i = 0.0000026;
  float I_k = *integral_acumulado + (MICROS_50HZ/2) * (e0 + e1); //I_k = I_{k-1} + T/2 * e_k + T/2 * e_{k-1}
  *integral_acumulado = I_k;
  
  float p =  control_p(e0);
  return p + k_i * I_k;
}

float control_pid(float* derivativo_acumulado, float* integral_acumulado, float e0, float e1){
  float k_d = 0.9;
  float D_k1;
  float D_k = (2.0/MICROS_50HZ) * (e0 - e1) - *derivativo_acumulado;
  *derivativo_acumulado = D_k;

  float pi = control_pi(integral_acumulado, e0, e1);
  return pi + k_d * D_k;
}