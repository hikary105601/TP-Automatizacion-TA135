#ifndef MiServo_h
#define MiServo_h

#include <Servo.h>

/* us de PWM. Los valores min y max están limitados al rango de movimiento de la barra; 
no corresponden a los valores extremos del servo motor (-90°, +90°)*/
#define SERVO_MIN 1250 // Lado del servo. Baja mas en este sentido
#define SERVO_MID 1475 // barra horizontal
#define SERVO_MAX 1820 // Lado sin  nada.

void inicializar_servo(Servo &servo);
void servo_angulo(Servo &servo, int angulo);
void servo_min(Servo &servo);
void servo_max(Servo &servo);

#endif