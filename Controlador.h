#ifndef Controlador_h
#define Cotrolador_h

float control_p(float e0);
float control_pi(float* integral_acumulado, float e0, float e1);
float control_pid(float* derivativo_acumulado, float* integral_acumulado, float e0, float e1);

#endif