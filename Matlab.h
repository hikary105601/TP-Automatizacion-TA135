#ifndef Matlab_h
#define Matlab_h

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

void matlab_test_imu(sensors_event_t a, sensors_event_t g);
void matlab_send_angles(float gyros, float accel, float filter);

#endif