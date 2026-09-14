#include <NewPing.h>
#include <Servo.h>
#include "MisConstantes.h"
#include "MiServo.h"
#include "IMU.h"

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

NewPing sonar(PIN_TRIG, PIN_ECHO, DISTANCIA_MAX);
Servo servo;
Adafruit_MPU6050 mpu;

sensors_event_t a, g, temp;
static float last_approx;

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
  inicializar_servo(servo); // se pone la barra en horizontal, es decir 90°
  last_approx = 90.0; // condición inicial de ángulo para cálculo de filtro

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

  if (t_actual - t_envio >= MICROS_ENVIO) {
    t_envio += MICROS_ENVIO;
    
    float gyro_angle = get_angle_gyro(g, last_approx);
    float accel_angle = get_angle_acceleration(a);
    float filter_angle = get_angle_filter(accel_angle, gyro_angle);

    //last_approx = gyro_angle; // diverge
    last_approx = filter_angle; // acotado

    //print_IMU(a,g,temp);
    matlab_send_angles(gyro_angle, accel_angle, filter_angle);
  }
}


void check_loop_freq(unsigned long t_inicio_loop, unsigned long t_fin_loop) {
  unsigned long t = t_fin_loop - t_inicio_loop;
  float t_seg = t / MICROS_EN_SEG;
  float f = 1 / t_seg;

  //Serial.print("Frecuencia loop en Hz: ");
  //Serial.println(f);
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
