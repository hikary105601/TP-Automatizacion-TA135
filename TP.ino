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

unsigned long t_inicio_loop;    // Cuando inicia cada ciclo de tareas
unsigned long t_actual;
unsigned long t_envio;  // ciclo transferencia datos a simulink
unsigned long t_servo;
static bool servo_up = true;

NewPing sonar(PIN_TRIG, PIN_ECHO, DISTANCIA_MAX);
Servo servo;
Adafruit_MPU6050 mpu;

sensors_event_t a, g, temp;
float filter_angle;

enum variables_regresion { // regresion lineal procesada en Matlab
  Y, // y_n
  U // u_n
};
float datos_regresion[2]; // [y_n u_n]

float referencia_distancia = 16.0;
float distancia;
float posicion;
float derivativo_acumulado;
float integral_acumulado;
float e[3]; // error angulo barra [e_n e_{n-1} e_{n-2}]
float u[3]; // accion de control  [u_n u_{n-1} u_{n-2}]

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
  delay(10);

  filter_angle = 0.0;
  //gyro_angle = 0.0;

  for(int i=0; i<2; i++){
    datos_regresion[i] = 0.0;
  }

  derivativo_acumulado = 0.0;
  integral_acumulado = 0.0;
  for(int i=0; i<3; i++){
    e[i] = 0.0;
    u[i] = 0.0;
  }

  for(int i=0; i<4; i++){
    datos_observador[i] = 0.0;
  }

  t_envio = 0;  // ciclo transferencia datos a simulink
  t_servo = 0;
  t_inicio_loop = micros();
  //t_loop_anterior = t_inicio_loop;
}

void loop() {
  t_actual = micros();

  if (t_actual - t_inicio_loop >= MICROS_50HZ) {
    t_inicio_loop += MICROS_50HZ;

    distancia = distancia_cm(sonar);
    mpu.getEvent(&a, &g, &temp);
    filter_angle = get_angle_filter(a, g, filter_angle);
    
    datos_observador[angReal] = filter_angle;
    datos_observador[velReal] = g.gyro.x;
    ///datos_observador[angEst] = ;
    //datos_observador[velReal] =

    posicion = referencia_distancia - distancia;

// TODO: implementar error y control con cola circular
    e[2] = e[1];
    e[1] = e[0];
    e[0] = posicion;

    u[2] = u[1];
    u[1] = u[0];
    u[0] = control_pd(&derivativo_acumulado, e);
    //u[0] = control_bilinear(e, u);

    servo_angulo(servo, u[0]);
  }

  if(t_actual - t_servo >= MICROS_SERVO){ // cuadrada servos
    t_servo += MICROS_SERVO;
    if(servo_up){
      referencia_distancia = 16.0;
      //servo_angulo(servo,-4.0);
      //servo_mid(servo);
      //datos_regresion[U] = servo.read()-90; // read devuelve el último ángulo entre 0 y 180. Resto 90 para centrar en 0
      servo_up = false;
    }
    else{
      //referencia_distancia = 6.0;
      //servo_angulo(servo, 12.0);
      //servo_mid(servo);
      //datos_regresion[U] = servo.read()-90; // read devuelve el último ángulo entre 0 y 180. Resto 90 para centrar en 0
      servo_up = true;
    } 
  }



  if (t_actual - t_envio >= MICROS_ENVIO) {
    t_envio += MICROS_ENVIO;

    datos_regresion[U] = filter_angle;
    datos_regresion[Y] = posicion;
    //matlab_send(datos_regresion, 2);

    float aux_send[2] = {16.0-distancia, referencia_distancia}; //posicion, referencia_distancia, u[0]
    matlab_send(aux_send, 2);

    //matlab_send(datos_observador, 4);
  }

}
