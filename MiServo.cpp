#include <Arduino.h>
#include "MiServo.h"

void inicializar_servo(Servo &servo){
  servo.writeMicroseconds(SERVO_MID);
  delay(2000);
}

void servo_angulo(Servo &servo, int angulo){ //valores de -90 a 90
/*Si el máximo y el mínimo no están a la misma distancia de SERVO_MID, habrá una precisión angular distinta hacia cada lado*/
  int angulo_map =  SERVO_MID;

  if(angulo < -90){
    angulo_map = SERVO_MIN;
  }else if(angulo > 90){
    angulo_map =  SERVO_MAX;  
  }else if(-90 <= angulo && angulo <= 0){
    angulo_map = map(angulo, -90, 0, SERVO_MIN, SERVO_MID);
  }else if (0 < angulo && angulo <= 90){
    angulo_map = map(angulo, 0, 90, SERVO_MID, SERVO_MAX);
  }
  
  servo.writeMicroseconds(angulo_map);
}

void servo_min(Servo &servo){ // Tarda aprox 0.75 segundos en hacer 180° => 1.333 Hz
  servo.writeMicroseconds(SERVO_MIN);
  }

void servo_max(Servo &servo){
  servo.writeMicroseconds(SERVO_MAX);
}