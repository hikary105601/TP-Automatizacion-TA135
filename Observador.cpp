#include "Observador.h"

float A_d[2][2] = {{0,1}, {-116.0, -18.29}};
float B_d[2] = {0, 72.768};
float C_d[2] = {1, 0};
float D_d = 0.0;
float L[2] = {0.5456, -0.0469};

float angulo_est(float* datos_observador){ // angulo de barra estimado =C.x^ + D.u
  return C_d[0]*datos_observador[angEst] + C_d[1]*datos_observador[velEst]; //  + D pero D=0
}

void actualizar_estimadas(float* datos_observador){ // asume datos_observador reales actualizados previamente
  float estimador[2]; //
  float A_aux[2]; // A_d x^_k
  float B_aux[2]; // B_d u_k
  float L_aux[2]; // L (y_k - y^_k)
  float e_y = datos_observador[angReal] - angulo_est(datos_observador); // asume que datos_observador[andReal] fue actualizado

  for(int i=0; i < 2; i++){
    A_aux[i] = A_d[i][0] * datos_observador[angEst] + A_d[i][1] * datos_observador[velEst];
    B_aux[i] = B_d[i] * datos_observador[angReal];
    L_aux[i] = L[i] * e_y;

    estimador[i] = A_aux[i] + B_aux[i] + L_aux[i];
  }
  datos_observador[angEst] = estimador[0];
  datos_observador[velEst] = estimador[1];
}
