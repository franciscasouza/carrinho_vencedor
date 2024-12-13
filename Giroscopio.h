#ifndef GIROSCOPIO_H
#define GIROSCOPIO_H

#include <Wire.h>
#include "MPU6050_light.h" // Certifique-se de que este arquivo existe e está no diretório correto

class Giroscopio {
private:
  MPU6050 mpu;
  float angX, angY, angZ;

public:
  Giroscopio();
  void begin();
  void update();
  float getAnguloX();
  float getAnguloY();
  float getAnguloZ();
  void girar(float graus);  // Valor positivo para direita, negativo para esquerda
  void imprimirAngulos();   // Imprime os valores de angX, angY e angZ
  
};

#endif
