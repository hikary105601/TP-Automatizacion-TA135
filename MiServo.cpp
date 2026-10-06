#include <Arduino.h>
#include "MiServo.h"

void inicializar_servo(Servo &servo){
  servo.writeMicroseconds(SERVO_MID);
  delay(2000);
}

void servo_angulo(Servo &servo, float angulo) { 
  float angulo_restringido = constrain(angulo, -90.0, 90.0);
  int pulso_us;

  if (angulo_restringido <= 0.0) {
    pulso_us = SERVO_MIN + ((angulo_restringido + 90.0) / 90.0) * (SERVO_MID - SERVO_MIN);
  } else {
    pulso_us = SERVO_MID + (angulo_restringido / 90.0) * (SERVO_MAX - SERVO_MID);
  }
  
  servo.writeMicroseconds(pulso_us);
}

void servo_min(Servo &servo){ // Tarda aprox 0.75 segundos en hacer 180° => 1.333 Hz
  servo.writeMicroseconds(SERVO_MIN);
  }

void servo_max(Servo &servo){
  servo.writeMicroseconds(SERVO_MAX);
}