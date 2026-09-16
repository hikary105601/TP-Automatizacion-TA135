#include <NewPing.h>
#include <Servo.h>
#include "MisConstantes.h"
#include "MiServo.h"
#include "IMU.h"
#include "Matlab.h"

#define PIN_POTE A0
#define PIN_TRIG 7
#define PIN_ECHO 6
#define PIN_SERVO 5   
#define DISTANCIA_MAX 60   //distancia máxima (cm) que detecta sensor ultrasónico
#define VEL_SONIDO 29.287  // us/cm

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

#define MUESTRAS 400
float transfer[MUESTRAS][2]; // 150 datos, columna para accion de control y angulo imu
int muestra = 0;

enum dato_angulo {
  SERVO,
  IMU
};


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

  t_inicio_loop = micros();
  t_loop_anterior = t_inicio_loop;
}

void loop() {
  t_actual = micros();
  if (t_actual - t_inicio_loop >= MICROS_50HZ) {
    t_inicio_loop += MICROS_50HZ;

    // ---------- Ahora sí ejecuto tareas ----------
    mpu.getEvent(&a, &g, &temp);
    //get_angle_filter(a, g);
  }

  if(t_actual - t_servo >= MICROS_SERVO){
    t_servo += MICROS_SERVO;
    if(servo_up){
      servo_min(servo);
      servo_up = false;
    }
    else{
      servo_max(servo);
      //servo_up = true;
    } 
  }

  if (t_actual - t_envio >= MICROS_ENVIO) {
    t_envio += MICROS_ENVIO;
    
    float gyro_angle = get_angle_gyro(g, gyro_angle);
    float accel_angle = get_angle_acceleration(a);
    filter_angle = get_angle_filter(a, g, filter_angle);

    //print_IMU(a,g,temp);
    //matlab_send_angles(gyro_angle, accel_angle, filter_angle);
/*
    if(muestra < MUESTRAS){
        transfer[muestra][SERVO] = servo.read();
        transfer[muestra][IMU] = filter_angle;
        muestra++;
    }
  }
  */
}


float angulo_pote(int lectura_pote) {
  return lectura_pote * (270.0 / 1023.0);
}

void distancia() {
  unsigned int uS = sonar.ping();  // Tiempo de vuelo ida y vuelta
  float distancia = uS / VEL_SONIDO;
  //Serial.print("Ping: ");
  //Serial.print(distancia);
  //Serial.println("cm")
}
