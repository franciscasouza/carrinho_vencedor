#ifndef CARRINHO_H
#define CARRINHO_H

#include <Arduino.h>
#include <Adafruit_TCS34725.h> // Biblioteca do sensor de cor
#include "Giroscopio.h"

class Carrinho {
private:
  // Motores
  int motorE1, motorE2, motorEV;
  int motorD1, motorD2, motorDV;
  int vel;

  // LEDs
  int pinVermelho; // Pino do LED vermelho
  int pinVerde;    // Pino do LED verde

  //botao
  // Botão
  int pinBotao;
  bool botaoPressionado;
  int estadoFase;

  // Sensores
  int pinSharp;
  int pinLinhaE, pinLinhaD;
  Adafruit_TCS34725 tcs;
  Giroscopio giroscopio;

  // Variáveis de leitura
  float distancia;
  uint16_t r, g, b, c; // Valores RGB do sensor de cor
  int leituraLinhaE, leituraLinhaD;

public:
  // Construtor
  Carrinho(int e1, int e2, int ev, int d1, int d2, int dv, int sharp, int linhaE, int linhaD, int vermelho, int verde,int botao);

  // Métodos básicos
  void begin(); // Inicializa os pinos e sensores
  void setVelocidade(int velocidade); // Define a velocidade dos motores
  void moverFrente(); // Move o carrinho para frente
  void moverTras(); // Move o carrinho para trás
  void girarEsquerda(); // Gira o carrinho para a esquerda
  void girarDireita(); // Gira o carrinho para a direita
  void parar(); // Para todos os motores

  // Sensores
  void lerSensores(); // Faz a leitura dos sensores
  String detectarCor(); // Detecta a cor com base nos valores RGB
  void verificarCor(); // Detecta a cor e aciona os LEDs

  // Controle com sensores
  void seguirLinha(); // Seguir a linha com os sensores de linha
  void pararPorDistancia(float distanciaLimite); // Para o carrinho com base na distância

  // Comunicação
  void enviarDadosParaESP32(); // Envia os dados lidos para o ESP32
  void imprimirDadosUmaLinha(); // Imprime os dados no monitor serial

  // Controle com giroscópio (opcional)
  void girarComGiroscopio(Giroscopio& giroscopio, float anguloDesejado);
  // void verificarBotao();
};

#endif
