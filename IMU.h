#ifndef IMU_H
#define IMU_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "MisConstantes.h"

void print_IMU(sensors_event_t a, sensors_event_t g, sensors_event_t temp);

void matlab_test_imu(sensors_event_t a, sensors_event_t g);
void matlab_send_angles(float gyros, float accel, float filter);

float get_angle_filter(float angle_accel, float angle_gyros);
float get_angle_acceleration(sensors_event_t a);
float get_angle_gyro(sensors_event_t g, float last_approx);

#endif