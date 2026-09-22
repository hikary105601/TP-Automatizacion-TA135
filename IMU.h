#ifndef IMU_H
#define IMU_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "MisConstantes.h"

void print_IMU(sensors_event_t a, sensors_event_t g, sensors_event_t temp);

float get_angle_filter(sensors_event_t a, sensors_event_t g, float last_filter);
float get_angle_acceleration(sensors_event_t a);
float get_angle_gyro(sensors_event_t g, float last_approx);

void tiempo_lectura_imu(Adafruit_MPU6050 &mpu, sensors_event_t a, sensors_event_t g, sensors_event_t temp);

#endif