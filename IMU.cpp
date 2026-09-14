#include "IMU.h"

static float dt = MICROS_50HZ / MICROS_EN_SEG ; // diferencial de tiempo para el giroscopio

void matlab_test_imu(sensors_event_t a, sensors_event_t g){
  Serial.write("abcd");
  byte * b = (byte *) &a.acceleration.x;
  Serial.write(b, sizeof(float));
  b = (byte *) &a.acceleration.y;
  Serial.write(b, sizeof(float));
  b = (byte *) &a.acceleration.z;
  Serial.write(b, sizeof(float));

  b = (byte *) &g.gyro.x;
  Serial.write(b, sizeof(float));
  b = (byte *) &g.gyro.y;
  Serial.write(b, sizeof(float));
  b = (byte *) &g.gyro.z;
  Serial.write(b, sizeof(float));
}

void matlab_send_angles(float gyros, float accel, float filter){
  Serial.write("abcd");
  byte * b = (byte *) &gyros;
  Serial.write(b, sizeof(float));
  b = (byte *) &accel;
  Serial.write(b, sizeof(float));
  b = (byte *) &filter;
  Serial.write(b, sizeof(float));
}

float get_angle_gyro(sensors_event_t g, float last_approx){ //,
  
  float x = degrees(g.gyro.x); // rad/s
  if(0 < g.gyro.x < 0.9) // se mueve lento = no se mueve?
    x = 0;
  float angulo_gx = last_approx + x * dt; 
  if(360 <= angulo_gx){
    angulo_gx -= 360.0;
  }
  else if(angulo_gx < 0.0){
    angulo_gx += 360.0;
  }
  /*
  Serial.print("Ángulo en X (giroscopio): ");
  Serial.print(angulo_gx);
  Serial.println("°");
  */
  return angulo_gx;
}

float get_angle_acceleration(sensors_event_t a){

  float y = a.acceleration.y; // lados
  float z = a.acceleration.z; // arriba
  float angulo = degrees(atan2(y,z));
  //Si quiero que el angulo este entre 0 y 360
  if(angulo < 0){
    angulo += 360.0;
  }
  /*
  Serial.print("Ángulo en X (acelerometro): ");
  Serial.print(angulo);
  Serial.println("°");
  */
  return angulo;
}

float get_angle_filter(float angle_accel, float angle_gyros){
  float alpha = 0.3; // alpha usualmente es mayor a 0.5, voy aumentando el valor hasta obtener el angulo correcto
  
  float angulo_obs = (1 - alpha) * angle_gyros + alpha * angle_accel;

  if(angulo_obs < 0){
    angulo_obs += 360.0;
  }
  /*
  Serial.print("Ángulo en X (con filtro): ");
  Serial.print(angulo_obs);
  Serial.println("°");
  */
  return angulo_obs;
}


void print_IMU(sensors_event_t a, sensors_event_t g, sensors_event_t temp){
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");
  
  Serial.println("");
  delay(1000);
}