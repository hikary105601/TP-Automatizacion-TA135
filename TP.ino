#include <NewPing.h>
#include <Servo.h>
#include "MisConstantes.h"
#include "Potenciometro.h"
#include "MiSonar.h"
#include "MiServo.h"
#include "IMU.h"
#include "Matlab.h"
#include "Controlador.h"

int calibration = CALIBRATION;
float offset_gyro = 0.0;
float gyro = 0.0;
float accel = 0.0;

unsigned long t_inicio_loop;    // Cuando inicia cada ciclo de tareas
unsigned long t_loop_anterior;  // Última ejecución de tareas
unsigned long t_actual;
unsigned long t_envio;  // ciclo transferencia datos a simulink
unsigned long t_servo;
static bool servo_up = true;

NewPing sonar(PIN_TRIG, PIN_ECHO, DISTANCIA_MAX);
Servo servo;
Adafruit_MPU6050 mpu;

sensors_event_t a, g, temp;
float filter_angle;
//float gyro_angle;

enum variables_regresion { // regresion lineal segundo orden y_n = c_y1 * y_{n-1} + c_y2 * y_{n-2} + c_u * u_n
  Y0, // y_n
  U0 // u_n
};
float datos_regresion[2]; // [y_n u_n]

float referencia_distancia = 16.0;
float distancia;
float derivativo_acumulado;
float integral_acumulado;
float e[3]; // error angulo barra [e_n e_{n-1} e_{n-2}]
float u[3]; // accion de control  [u_n u_{n-1} u_{n-2}]

float A_d[2][2] = {{0,1}, {-132.8, -20.92}};
float B_d[2] = {0, 82.663};
float C_d[2] = {1, 0};
float D_d = 0.0;
float L[2] = {0.7590, -0.4596};
enum variables_observador{
  angEst,      // angulo de barra estimado
  angReal,     // angulo imu
  velEst,      // velocidad angular estimada
  velReal      // velocidad angular giroscopo
};
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

  t_inicio_loop = micros();
  t_loop_anterior = t_inicio_loop;
}

void loop() {
  t_actual = micros();
  if (t_actual - t_inicio_loop >= MICROS_50HZ) {
    t_inicio_loop += MICROS_50HZ;

    mpu.getEvent(&a, &g, &temp);
    //distancia = distancia_cm(sonar);
    filter_angle = get_angle_filter(a, g, filter_angle);
    datos_observador[velReal] = g.gyro.x;

    e[2] = e[1];
    e[1] = e[0];
    //e[0] = referencia_distancia - distancia;
    e[0] = 0.0 - filter_angle;

    u[2] = u[1];
    u[1] = u[0];
    //u[0] = //control_pid(&derivativo_acumulado, &integral_acumulado, e);
    u[0] = control_bilinear(e, u);

    //servo_angulo(servo, u[0]);

    /*y_est = C_d[0]*datos_observador[angEst] + C_d[1]*datos_observador[velEst];
    for(int i=0; i < 2; i++){ //TODO: actualizar para usar vector datos
      float aux = 0.0;
      for(int j=0; j < 2; j++){
        aux += A_d[i][j] * x_est[j];
      }
      x_est[i] = aux + B_d[i] * u[0] + L[i] *(filter_angle - y_est);
    }
    datos_observador[velEst] = x_est[1];*/
  }


  if(t_actual - t_servo >= MICROS_SERVO){ // cuadrada servos
    t_servo += MICROS_SERVO;
    if(servo_up){
      servo_mid(servo);
      datos_regresion[U0] = servo.read() - 91; // read devuelve el último ángulo entre 0 y 180. Resto 90 para centrar en 0
      servo_up = false;
    }
    else{
      servo_max(servo);
      datos_regresion[U0] = servo.read() - 91; // read devuelve el último ángulo entre 0 y 180. Resto 90 para centrar en 0
      servo_up = true;
    } 
  }

  if (t_actual - t_envio >= MICROS_ENVIO) {
    t_envio += MICROS_ENVIO;

    datos_regresion[Y0] = filter_angle;
    matlab_send(datos_regresion, 2);

    //matlab_send(datos_observador, 4);
  }

}
