#ifndef IMU_H
#define IMU_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#define MICROS_ENVIO 20000

void print_IMU(sensors_event_t a, sensors_event_t g, sensors_event_t temp);
void matlab_send(sensors_event_t a, sensors_event_t g);

#endif