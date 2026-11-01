#include "Herramientas.h"

// ================= MATLAB =================

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

void matlab_send(float* datos, int cant_datos){
  byte * b = NULL;

  Serial.write("abcd");
  for(int i=0; i<cant_datos; i++){
    b = (byte *) &datos[i];
    Serial.write(b, sizeof(float));
  }
  
}

// ================= COLA CIRCULAR =================
void cola_init(circular_t* c){
  for(int i=0; i<c->size; i++){
    c->dato[i] = 0.0;
  }
  c->newest_index = 0;
  c->cant = 0;
}

circular_t crear_cola(int cola_size){
  circular_t c;
  c.size = cola_size;
  c.dato = (float*) malloc(cola_size * sizeof(float));
  cola_init(&c);
  return c;
}

void encolar(float dato, circular_t* cola){
  if(cola->size <= 0)
    return;

  cola->dato[cola->newest_index] = dato;
  cola->newest_index++;
  if(cola->newest_index == cola->size)
    cola->newest_index = 0;
  if(cola->cant < cola->size)
    cola->cant++;
}

float dato_n(circular_t c, int n){ // dato n posiciones atrás
  if(n < 0 || n > c.cant) // datos más antiguos que la cantidad guardada
    return 0.0;
  
  int index = c.newest_index - 1 - n;
  if(index < 0)
    index += c.size;

  return c.dato[index];
} 