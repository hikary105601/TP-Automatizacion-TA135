#ifndef MiConstantes_h
#define MiConstantes_h

#define PIN_POTE A0
#define PIN_TRIG 7
#define PIN_ECHO 6
#define PIN_SERVO 5   
#define DISTANCIA_MAX 40   //distancia máxima (cm) que detecta sensor ultrasónico = longitud de la barra
#define VEL_SONIDO 29.287  // us/cm

#define CALIBRATION 500

#define MICROS_EN_SEG 1000000.0

#define MICROS_50HZ 20000
#define MICROS_1HZ 1000000

#define MICROS_ENVIO 20000 // Matlab 0.02 seg
#define MICROS_SERVO 5000000 // 2s
#define MICROS_SONAR 20000 // modificar en caso de ruido por rebotes

#endif