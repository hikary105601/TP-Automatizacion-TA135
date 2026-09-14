#ifndef MiServo_h
#define MiServo_h

#include <Servo.h>

#define SERVO_MIN 550 // us PWM duty cycle 
#define SERVO_MAX 2400
#define SERVO_MID 1475

void inicializar_servo(Servo &servo);
void mover_servo_angulo(int angulo);
void servo_min(Servo &servo);
void servo_max(Servo &servo);

#endif