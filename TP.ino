#include <NewPing.h>
#include <Servo.h>
#include "MisConstantes.h"
#include "Potenciometro.h"
#include "MiSonar.h"
#include "MiServo.h"
#include "IMU.h"
#include "Matlab.h"
#include "Controlador.h"

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
float gyro_angle;

enum variables_regresion { // regresion lineal segundo orden y_n = c_y1 * y_{n-1} + c_y2 * y_{n-2} + c_u * u_n
  Y0, // y_n
  Y1, // y_{n-1}
  Y2, // y_{n-2}
  U0 // u_n
};
float datos_regresion[4]; // [y_n y_{n-1} y_{n-2} u_n]

float referencia_distancia = 16.0;

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
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  Serial.println("");
  delay(100);

  servo.attach(PIN_SERVO, SERVO_MIN, SERVO_MAX);
  inicializar_servo(servo); // se pone la barra en horizontal

  filter_angle = 0.0;
  gyro_angle = 0.0;

  for(int i=0; i<4; i++){
    datos_regresion[i] = 0.0;
  }

  t_inicio_loop = micros();
  t_loop_anterior = t_inicio_loop;
}

void loop() {
  t_actual = micros();
  if (t_actual - t_inicio_loop >= MICROS_50HZ) {
    t_inicio_loop += MICROS_50HZ;

    mpu.getEvent(&a, &g, &temp);    
    float e0 = referencia_distancia - distancia_cm(sonar);
    Serial.println(control_p(e0));
    servo_angulo(servo, control_p(e0));
  }
/*
  if(t_actual - t_servo >= MICROS_SERVO){ // cuadrada servos
    t_servo += MICROS_SERVO;
    if(servo_up){
      servo_min(servo);
      datos_regresion[U0] = servo.read() - 90; // read devuelve el último ángulo entre 0 y 180. Resto 90 para centrar en 0
      servo_up = false;
    }
    else{
      servo_max(servo);
      datos_regresion[U0] = servo.read() - 90; // read devuelve el último ángulo entre 0 y 180. Resto 90 para centrar en 0
      servo_up = true;
    } 
  }
*/
/*
  if (t_actual - t_envio >= MICROS_ENVIO) {
    t_envio += MICROS_ENVIO;
    
    filter_angle = get_angle_filter(a, g, filter_angle);

    datos_regresion[Y2] =  datos_regresion[Y1];
    datos_regresion[Y1] = datos_regresion[Y0];
    datos_regresion[Y0] = filter_angle;
    
    matlab_send_regresion(datos_regresion, 4);
  }
  */
}
