#include <NewPing.h>
#include <Servo.h>
#include "MisConstantes.h"
#include "Potenciometro.h"
#include "MiSonar.h"
#include "MiServo.h"
#include "IMU.h"
#include "Matlab.h"
#include "Controlador.h"
#include "Observador.h"

unsigned long t_inicio_loop;
unsigned long t_actual;
unsigned long t_envio;  // ciclo transferencia datos a simulink
unsigned long t_servo;
unsigned long t_sonar;
static bool servo_up = true;

NewPing sonar(PIN_TRIG, PIN_ECHO, DISTANCIA_MAX);
Servo servo;
Adafruit_MPU6050 mpu;

sensors_event_t a, g, temp;
float filter_angle;
float desvio_accel;

enum variables_regresion {Y, U};
float datos_regresion[2]; // [y_n u_n]

float referencia_distancia = 16.0;
float distancia;
float posicion;
float derivativo_acumulado;
float integral_acumulado;
#define CIRCULAR 3
float e[3]; // error angulo barra [e_n e_{n-1} e_{n-2}]
int e_index;
float u[3]; // accion de control  [u_n u_{n-1} u_{n-2}]
int u_index;

float datos_observador[4]; 

void setup() {
  Serial.begin(115200);

  while (!Serial) delay(10);  // will pause Zero, Leonardo, etc until serial console opens
  if (!mpu.begin()) {         // Try to initialize!
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); //2000
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  Serial.println("");
  delay(100);

  servo.attach(PIN_SERVO, SERVO_MIN, SERVO_MAX);
  inicializar_servo(servo); // se pone la barra en horizontal
  delay(200);

  filter_angle = 0.0;
  float suma_accel = 0.0;
  for (int i = 0; i < 100; i++) { // calcula desvio para calibrar a 0°
    mpu.getEvent(&a, &g, &temp);
    suma_accel += get_angle_acceleration(a);
  }
  desvio_accel = suma_accel / 100;

  for(int i=0; i<2; i++){
    datos_regresion[i] = 0.0;
  }

  derivativo_acumulado = 0.0;
  integral_acumulado = 0.0;

  for(int i=0; i<CIRCULAR; i++){
    e[i] = 0.0;
    u[i] = 0.0;
  }
  e_index = 0;
  u_index = 0;

  for(int i=0; i<4; i++){
    datos_observador[i] = 0.0;
  }

  t_envio = 0;  // ciclo transferencia datos a simulink
  t_servo = 0;
  t_sonar = 0;

  t_inicio_loop = micros();
}

void loop() {
  t_actual = micros();

  if (t_actual - t_sonar >= MICROS_SONAR) {
    t_sonar += MICROS_SONAR;
    distancia = distancia_cm(sonar);
  }

  if (t_actual - t_inicio_loop >= MICROS_50HZ) {
    t_inicio_loop += MICROS_50HZ;

    mpu.getEvent(&a, &g, &temp);
    filter_angle = get_angle_filter(a, g, filter_angle, desvio_accel);
    
    datos_observador[angReal] = filter_angle;
    datos_observador[velReal] = g.gyro.x;
    actualizar_estimadas(datos_observador); // actualizar angReal antes!

    posicion = referencia_distancia - distancia;

    e[e_index] = posicion;;
    e_index = (e_index+1)%CIRCULAR;

    u[u_index] = control_pd(&derivativo_acumulado, e);
    u_index = (u_index+1)%CIRCULAR;

    //servo_angulo(servo, u[u_index]);
  }

  if(t_actual - t_servo >= MICROS_SERVO){ // cuadrada servos
    t_servo += MICROS_SERVO;
    if(servo_up){
      //referencia_distancia = 16.0;
      servo_mid(servo);
      //filter_angle = 0.0;
      //datos_regresion[U] = servo.read()-90; // read devuelve el último ángulo entre 0 y 180. Resto 90 para centrar en 0
      servo_up = false;
    }
    else{
      //referencia_distancia = 6.0;
      servo_max(servo);
      //filter_angle = 6.5;
      //servo_angulo(servo, 18.0);
      //datos_regresion[U] = servo.read()-90; // read devuelve el último ángulo entre 0 y 180. Resto 90 para centrar en 0
      servo_up = true;
    } 
  }



  if (t_actual - t_envio >= MICROS_ENVIO) {
    t_envio += MICROS_ENVIO;

    datos_regresion[U] = filter_angle;
    datos_regresion[Y] = posicion;
    matlab_send(datos_regresion, 2);

    //float aux_send[2] = {16.0-distancia, referencia_distancia}; //posicion, referencia_distancia, u[0]
    //matlab_send(aux_send, 2);

    //matlab_send(datos_observador, 4);
  }

}