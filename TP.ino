// IMU
#define MICROS_EN_SEG 1000000
#define MICROS_50HZ 20000
#define MICROS_100HZ 10000

unsigned long t_inicio_loop; // Cuando inicia cada ciclo de tareas
unsigned long t_loop_anterior; // Última ejecución de tareas
unsigned long t_actual;

void setup() {
  Serial.begin(115200); // Suficientemente alto para que carguen los print
  t_inicio_loop = micros();
  t_loop_anterior = t_inicio_loop;
}

void loop() {
  t_actual = micros();
  if(t_actual - t_inicio_loop >= MICROS_50HZ){
    check_loop_freq(t_loop_anterior,t_actual); // Verifico 20ms (50Hz) desde última ejecución
    t_loop_anterior = t_actual;
    t_inicio_loop += MICROS_50HZ;

    // ---------- Ahora sí ejecuto tareas ----------

  }
}

void check_loop_freq(unsigned long t_inicio_loop, unsigned long t_fin_loop){
  unsigned long t = t_fin_loop - t_inicio_loop;
  float t_seg = t / MICROS_EN_SEG;
  float f = 1/t_seg;
  
  Serial.print("Frecuencia loop en Hz: ");
  Serial.println(f);
}

