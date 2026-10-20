#include "Observador.h"

float A_d[2][2] = {{0,1}, {-116.0, -18.29}};
float B_d[2] = {0, 72.768};
float C_d[2] = {1, 0};
float D_d = 0.0;
float L[2] = {0.5456, -0.0469};

float estimador_salida(float* datos_observador){ // angulo de barra estimado
  return datos_observador[angEst]; // C_d[0]*datos_observador[angEst] + C_d[1]*datos_observador[velEst];
}

float* estimador_variables(float* datos_observador, float salida_real){
  float siguiente_estimador[2];
  float A_aux[2]; // A_d x^_k
  float B_aux[2]; // B_d u_k
  float L_aux[2]; // L (y_k - y^_k)
  float e_y = salida_real - estimador_salida(datos_observador);

  for(int i=0; i < 2; i++){
    A_aux[i] = A_d[i][0] * datos_observador[angEst] + A_d[i][1] * datos_observador[velEst];
    B_aux[i] = B_d[i] * datos_observador[angReal];
    L_aux[i] = L[i] * e_y;

    siguiente_estimador[i] = A_aux[i] + B_aux[i] + L_aux[i];
  }

  return siguiente_estimador;
}
