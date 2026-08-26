#define PIN_POTE A0
#define MICROS_EN_SEG 1000000.0
#define MICROS_50HZ 20000

void check_loop_freq(unsigned long t_inicio_loop, unsigned long t_fin_loop);
float angulo_pote(int pote);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  unsigned long t_inicio_loop = micros();

  unsigned long t_previo = micros();
  int lectura_pote = analogRead(PIN_POTE);
  unsigned long t_actual = micros();

  unsigned long t_transcurrido = t_actual - t_previo;

  unsigned long t_idle = MICROS_50HZ - t_transcurrido; 
  delayMicroseconds(t_idle);
  
  unsigned long t_fin_loop = micros();
  check_loop_freq(t_inicio_loop,t_fin_loop);  

}

void check_loop_freq(unsigned long t_inicio_loop, unsigned long t_fin_loop){
  unsigned long t = t_fin_loop - t_inicio_loop;
  float t_seg = t / MICROS_EN_SEG;
  float f = 1/t_seg;
  
  Serial.print("Frecuencia loop en Hz: ");
  Serial.println(f);
}

float angulo_pote(int pote){
  float angulo = pote * (270.0/1023.0);
  Serial.println("Ángulo");
  Serial.println(angulo);
  return angulo;
  }