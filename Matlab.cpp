#include "Matlab.h"

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


void matlab_send_regresion(float datos_regresion[], int cant_datos){
  byte * b = NULL;

  Serial.write("abcd");
  for(int i=0; i<cant_datos; i++){
    b = (byte *) &datos_regresion[i];
    Serial.write(b, sizeof(float));
  }
  
}