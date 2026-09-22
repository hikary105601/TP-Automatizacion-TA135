#include "IMU.h"

static float dt = MICROS_50HZ / MICROS_EN_SEG ; // diferencial de tiempo para el giroscopio



float get_angle_gyro(sensors_event_t g, float last_approx){ //,
  
  float x = degrees(g.gyro.x); // rad/s
  float angulo_gx = last_approx + x * dt; 

  return angulo_gx;
}

float get_angle_acceleration(sensors_event_t a){

  float y = a.acceleration.y; // lados
  float z = a.acceleration.z; // arriba
  float angulo = degrees(atan2(y,z)); // va de -180 a +180

  return angulo; // 0 si componentes hacia arriba
}

float get_angle_filter(sensors_event_t a, sensors_event_t g, float last_filter){
  float alpha = 0.05;
  
  float angle_gyros = get_angle_gyro(g, last_filter);
  float angle_accel = get_angle_acceleration(a);
  
  return (1 - alpha) * angle_gyros + alpha * angle_accel;
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

void tiempo_lectura_imu(Adafruit_MPU6050 &mpu, sensors_event_t a, sensors_event_t g, sensors_event_t temp){
    unsigned long antes_imu = micros();
    mpu.getEvent(&a, &g, &temp);
    unsigned long despues_imu = micros();
    Serial.print("Tiempo de lectura de imu: ");
    Serial.println(despues_imu - antes_imu);
}