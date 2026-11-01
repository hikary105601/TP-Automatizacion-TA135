#include <NewPing.h>
#include <Servo.h>
#include "MisConstantes.h"
#include "Sensores.h"
#include "MiServo.h"
#include "Herramientas.h"
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

float referencia_d = 16.0;
float referencia_p = 0.0;
float distancia;
float posicion;
float derivativo_acumulado;
float integral_acumulado;

circular_t e = crear_cola(3); // posición de carrito
circular_t u = crear_cola(3); // angulo de servo

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
  
  cola_init(&e);
  cola_init(&u);

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
    
    //datos_observador[angReal] = filter_angle;
    //datos_observador[velReal] = g.gyro.x;
    //actualizar_estimadas(datos_observador); // actualizar angReal antes!

    posicion = referencia_d - distancia;
    float error_pos = referencia_p - posicion;
    encolar(error_pos, &e);
    float e0 = dato_n(e,0);
    float e1 = dato_n(e,1);
    float control = control_pd(&derivativo_acumulado,e0,e1);
    
    encolar(control, &u);

    servo_angulo(servo, dato_n(u,0));
  }

  if(t_actual - t_servo >= MICROS_SERVO){ // cuadrada servos
    t_servo += MICROS_SERVO;
    if(servo_up){
      //referencia_p = 0.0;
      //servo_mid(servo);
      //servo_angulo(servo, -10.0);
      //datos_regresion[U] = servo.read()-90; // read devuelve el último ángulo entre 0 y 180. Resto 90 para centrar en 0
      servo_up = false;
    }
    else{
      //referencia_p = 10.0;
      //servo_max(servo);
      //servo_angulo(servo, 10.0);
      //datos_regresion[U] = servo.read()-90; // read devuelve el último ángulo entre 0 y 180. Resto 90 para centrar en 0
      servo_up = true;
    } 
  }


  if (t_actual - t_envio >= MICROS_ENVIO) {
    t_envio += MICROS_ENVIO;

    //datos_regresion[U] = filter_angle;
    //datos_regresion[Y] = ;
    
    float aux = 16.0 - distancia;
    matlab_send(&aux,1);

    //matlab_send(datos_observador, 4);
  }

}