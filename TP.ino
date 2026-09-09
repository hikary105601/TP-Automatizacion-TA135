// IMU
#include "IMU.h"

#define MICROS_EN_SEG 1000000.0
#define MICROS_50HZ 20000


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
  
  Serial.print("Frecuencia loop en Hz: ");
  Serial.println(f);
}

