#include "IMU.h"

void matlab_send(sensors_event_t a, sensors_event_t g){
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
