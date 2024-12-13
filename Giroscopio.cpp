#include "Giroscopio.h"

Giroscopio::Giroscopio() : mpu(Wire), angX(0), angY(0), angZ(0) {}

void Giroscopio::begin() {
  mpu.begin();
  mpu.calcOffsets(); // Realiza a calibração inicial do giroscópio
}

void Giroscopio::update() {
  mpu.update();
  angX = mpu.getAngleX();
  angY = mpu.getAngleY();
  angZ = mpu.getAngleZ();
}

float Giroscopio::getAnguloX() {
  return angX;
}

float Giroscopio::getAnguloY() {
  return angY;
}

float Giroscopio::getAnguloZ() {
  return angZ;
}

void Giroscopio::imprimirAngulos() {
  Serial.print("Angulo X: "); Serial.print(angX, 2); Serial.print("°");
  Serial.print(" | Angulo Y: "); Serial.print(angY, 2); Serial.print("°");
  Serial.print(" | Angulo Z: "); Serial.print(angZ, 2); Serial.println("°");
}

void Giroscopio::girar(float anguloDesejado) {
  float anguloAtual = 0;
  unsigned long tempoAnterior = millis();
  unsigned long tempoAtual;

  Serial.println("Iniciando rotação...");

  // Loop para calcular o ângulo baseado no giroscópio
  while (abs(anguloAtual) < abs(anguloDesejado)) {
    // Atualiza os valores do giroscópio
    mpu.update();

    // Calcula o tempo decorrido
    tempoAtual = millis();
    float deltaT = (tempoAtual - tempoAnterior) / 1000.0; // Converte para segundos
    tempoAnterior = tempoAtual;

    // Integração da velocidade angular no eixo Z
    float velocidadeAngularZ = mpu.getGyroZ();
    anguloAtual += velocidadeAngularZ * deltaT;

    // Exibe o ângulo atual no monitor serial
    Serial.print("Angulo Atual: ");
    Serial.println(anguloAtual);
  }

   // Para o carrinho após alcançar o ângulo
  Serial.println("Rotação concluída!");
}
