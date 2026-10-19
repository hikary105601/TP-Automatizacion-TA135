#ifndef MiServo_h
#define MiServo_h

#include <Servo.h>

/* us de PWM. Los valores min y max están limitados al rango de movimiento de la barra; 
no corresponden a los valores extremos del servo motor (-90°, +90°)*/
#define SERVO_MIN 1270 // Lado del servo. Baja mas en este sentido
#define SERVO_MID 1565 
#define SERVO_MAX 1860 // Lado sin  nada.
#define US_GRADO (2400.0-550.0)/180.0 // resolucion PWM

void inicializar_servo(Servo &servo);
void servo_angulo(Servo &servo, float angulo);
void servo_min(Servo &servo);
void servo_max(Servo &servo);
void servo_mid(Servo &servo);

#endif