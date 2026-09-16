#ifndef MiServo_h
#define MiServo_h

#include <Servo.h>

// us de PWM
#define SERVO_MIN 1250 // lado del servo. Baja mas en este sentido
#define SERVO_MID 1475 // barra horizontal
#define SERVO_MAX 1820 // Lado sin  nada.

void inicializar_servo(Servo &servo);
//void mover_servo_angulo(int angulo);
void servo_min(Servo &servo);
void servo_max(Servo &servo);

#endif