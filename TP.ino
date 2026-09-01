// IMU

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define MICROS_EN_SEG 1000000.0
#define MICROS_100HZ 10000
#define MICROS_ENVIO 20000 // 50Hz

unsigned long t_inicio_loop; // Cuando inicia cada ciclo de tareas
unsigned long t_loop_anterior; // Última ejecución de tareas
unsigned long t_actual;
unsigned long t_envio; // ciclo transferencia datos a simulink


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
}

void loop() {
  t_actual = micros();
  if(t_actual - t_inicio_loop >= MICROS_100HZ){
    //check_loop_freq(t_loop_anterior,t_actual); // Verifico 20ms (50Hz) desde última ejecución
    //t_loop_anterior = t_actual;
    t_inicio_loop += MICROS_100HZ;

  // ---------- Ahora sí ejecuto tareas ----------
  mpu.getEvent(&a, &g, &temp);
  // print_IMU(a, g, temp);
  }

  if(t_actual - t_envio >= MICROS_ENVIO){
    t_envio += MICROS_ENVIO;
    matlab_send(a.acceleration.x, a.acceleration.y, a.acceleration.z);
  }
}

void matlab_send(float dato1, float dato2, float dato3){
  Serial.write("abcd");
  byte * b = (byte *) &dato1;
  Serial.write(b,sizeof(float));
  b = (byte *) &dato2;
  Serial.write(b,sizeof(float));
  b = (byte *) &dato3;
  Serial.write(b,sizeof(float));
  //etc con mas datos tipo float. Tambien podría pasarse como parámetro a esta funcion un array de floats.
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
//  delay(1000);
}

void check_loop_freq(unsigned long t_inicio_loop, unsigned long t_fin_loop){
  unsigned long t = t_fin_loop - t_inicio_loop;
  float t_seg = t / MICROS_EN_SEG;
  float f = 1/t_seg;
  
  Serial.print("Frecuencia loop en Hz: ");
  Serial.println(f);
}

