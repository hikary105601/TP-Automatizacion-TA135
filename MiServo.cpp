#include <Arduino.h>
#include "MiServo.h"

void inicializar_servo(Servo &servo){
  servo.writeMicroseconds(SERVO_MID);
  delay(2000);
}
/*
void mover_servo_angulo(Servo &servo, int angulo){
  int aux = map(angulo, -, 270, -90, 90);
  servo.write(aux);
}
*/
void servo_min(Servo &servo){ // Tarda aprox 0.75 segundos en hacer 180° => 1.333 Hz
  servo.writeMicroseconds(SERVO_MIN);
  //delay(2000);
  }

void servo_max(Servo &servo){
  servo.writeMicroseconds(SERVO_MAX);
  //delay(2000);
}