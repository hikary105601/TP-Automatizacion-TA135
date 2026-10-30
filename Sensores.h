#ifndef Sensores_h
#define Sensores_h

// ================= iMU =================

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "MisConstantes.h"

float get_angle_gyro(sensors_event_t g, float last_approx);
float get_angle_acceleration(sensors_event_t a);
float get_angle_filter(sensors_event_t a, sensors_event_t g, float last_filter, float desvio_a);

unsigned long tiempo_lectura_imu(Adafruit_MPU6050 &mpu, sensors_event_t a, sensors_event_t g, sensors_event_t temp);

void print_IMU(sensors_event_t a, sensors_event_t g, sensors_event_t temp);

// ================= SONAR =================

#include <NewPing.h>

float distancia_cm(NewPing &sonar);
unsigned long tiempo_lectura_sonar(NewPing &sonar);

// ================= POTE =================

float angulo_pote(int lectura_pote);

#endif