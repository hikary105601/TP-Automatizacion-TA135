#include "Sensores.h"
#include "MisConstantes.h"

// ================= IMU =================

static float dt = MICROS_50HZ / MICROS_EN_SEG; // diferencial de tiempo para el giroscopio

float get_angle_gyro(sensors_event_t g, float last_approx){ //,
  
  float x = -degrees(g.gyro.x); // rad/s
  float angulo_gx = last_approx + x * dt; 

  return angulo_gx;
}

float get_angle_acceleration(sensors_event_t a){

  float y = a.acceleration.y; // horizontal
  float z = -a.acceleration.z; // vertical
  float angulo = degrees(atan2(y,z)); // va de -180 a +180

  return angulo; // 0 si componentes hacia abajo
}

float get_angle_filter(sensors_event_t a, sensors_event_t g, float last_filter, float desvio_a){
  float alpha = 0.05;
  
  float angle_gyros = get_angle_gyro(g, last_filter);
  float angle_accel = get_angle_acceleration(a) - desvio_a + 0.58;
  return (1 - alpha) * angle_gyros + alpha * angle_accel;
}

unsigned long tiempo_lectura_imu(Adafruit_MPU6050 &mpu, sensors_event_t a, sensors_event_t g, sensors_event_t temp){
    unsigned long antes_imu = micros();
    mpu.getEvent(&a, &g, &temp);
    unsigned long despues_imu = micros();
    return despues_imu - antes_imu;
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

// ================= SONAR =================
float distancia_cm(NewPing &sonar){
  unsigned int uS = sonar.ping(DISTANCIA_MAX);  // Tiempo de vuelo ida y vuelta
  return (uS/2) / VEL_SONIDO;
}

unsigned long tiempo_lectura_sonar(NewPing &sonar){ // microsegundos
  unsigned long antes_sonar = micros();
  sonar.ping(DISTANCIA_MAX);
  unsigned long despues_sonar = micros();
  return despues_sonar - antes_sonar;
}

// ================= POTE =================
float angulo_pote(int lectura_pote) {
  return lectura_pote * (270.0 / 1023.0);
}

unsigned long tiempo_lectura_pote(){ // microsegundos
  unsigned long antes_pote = micros();
  int valor_pote = analogRead(PIN_POTE);
  unsigned long despues_pote = micros();
  return despues_pote - antes_pote;
}