#ifndef Controlador_h
#define Cotrolador_h

float control_bilinear(float* e, float* u);

float control_p(float* e);
float control_pi(float* integral_acumulado, float* e);
float control_pid(float* derivativo_acumulado, float* integral_acumulado, float* e);

#endif