#ifndef Herramientas_h
#define Herramientas_h

// ================= MATLAB =================

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

void matlab_test_imu(sensors_event_t a, sensors_event_t g);
void matlab_send_angles(float gyros, float accel, float filter);
void matlab_send(float* datos, int cant_datos);

// ================= COLA CIRCULAR =================
struct circular_t{
  float* dato;
  int size;
  int newest_index; 
  int oldest_index; 
};

circular_t crear_cola(int cola_size);
void cola_init(circular_t c);
void encolar(float dato, circular_t* cola);
float dato_reciente(circular_t c);
float dato_n(circular_t c, int n); // dato n posiciones atrás

#endif