#include <NewPing.h>
#include <Servo.h>
#include "IMU.h"

#define PIN_POTE A0
#define PIN_TRIG 7
#define PIN_ECHO 6
#define PIN_SERVO 5
#define DISTANCIA_MAX 60 //distancia máxima (cm) que detecta sensor ultrasónico
#define MICROS_EN_SEG 1000000.0
#define MICROS_50HZ 20000
#define MICROS_1HZ 1000000
#define VEL_SONIDO 29.287 // us/cm
#define SERVO_MIN 550 // us PWM duty cycle 
#define SERVO_MAX 2400 // us PWM duty cycle 

unsigned long t_inicio_loop; // Cuando inicia cada ciclo de tareas
unsigned long t_loop_anterior; // Última ejecución de tareas
unsigned long t_actual;
unsigned long t_envio; // ciclo transferencia datos a simulink

NewPing sonar(PIN_TRIG, PIN_ECHO, DISTANCIA_MAX); 
Servo servo;

Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

void setup() {
  Serial.begin(115200); // Suficientemente alto para que carguen los print
  
  while (!Serial) delay(10); // will pause Zero, Leonardo, etc until serial console opens
  if (!mpu.begin()) { // Try to initialize!
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

  t_inicio_loop = micros();
  t_loop_anterior = t_inicio_loop;
  servo.attach(PIN_SERVO, SERVO_MIN, SERVO_MAX);
}

void loop() {
  t_actual = micros();
  if(t_actual - t_inicio_loop >= MICROS_50HZ){
    t_inicio_loop += MICROS_50HZ;

  // ---------- Ahora sí ejecuto tareas ----------
  mpu.getEvent(&a, &g, &temp);
  
  }

  if(t_actual - t_envio >= MICROS_ENVIO){
    t_envio += MICROS_ENVIO;
    matlab_send(a,g);
    //print_IMU(a, g, temp);
  }
}

void check_loop_freq(unsigned long t_inicio_loop, unsigned long t_fin_loop){
  unsigned long t = t_fin_loop - t_inicio_loop;
  float t_seg = t / MICROS_EN_SEG;
  float f = 1/t_seg;
  
  //Serial.print("Frecuencia loop en Hz: ");
  //Serial.println(f);
}

float angulo_pote(int lectura_pote){
  return lectura_pote * (270.0/1023.0);
}

void distancia(){
  unsigned int uS = sonar.ping(); // Tiempo de vuelo ida y vuelta
  float distancia = uS/VEL_SONIDO;
  //Serial.print("Ping: ");
  //Serial.print(distancia); 
  //Serial.println("cm")
}

void mover_servo_angulo(int angulo){
  int aux = min(angulo, 180); // if value is < 200 it's treated as an angle, otherwise as pulse width in microseconds
  Serial.println(aux);
  servo.write(aux);                  // sets the servo position according to the scaled value
}

void servo_180(){ // Tarda aprox 0.75 segundos en hacer 180°
  servo.writeMicroseconds(SERVO_MIN);
  delay(2000);
  servo.writeMicroseconds(SERVO_MAX);
  delay(2000);
  }