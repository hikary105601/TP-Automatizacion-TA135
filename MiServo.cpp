#include <Arduino.h>
#include "MiServo.h"

void inicializar_servo(Servo &servo){
  servo.writeMicroseconds(SERVO_MID);
  delay(2000);
}

void servo_angulo(Servo &servo, float angulo) {
  float ref_pulso = SERVO_MID + angulo * US_GRADO;
  int pulso_restringido = constrain(ref_pulso, SERVO_MIN, SERVO_MAX);

  servo.writeMicroseconds(pulso_restringido);
}

void servo_min(Servo &servo){ // Tarda aprox 0.75 segundos en hacer 180° => 1.333 Hz
  servo.writeMicroseconds(SERVO_MIN);
  }

void servo_max(Servo &servo){
  servo.writeMicroseconds(SERVO_MAX);
}

void servo_mid(Servo &servo){
  servo.writeMicroseconds(SERVO_MID);
}