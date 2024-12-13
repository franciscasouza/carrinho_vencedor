#include "Carrinho.h"
#include "Giroscopio.h"
#include <Servo.h>

Servo garra;

// Definição dos pinos
#define MOTOR_E1 2
#define MOTOR_E2 4
#define MOTOR_EV 3
#define MOTOR_D1 7
#define MOTOR_D2 5
#define MOTOR_DV 6
#define SHARP_PIN A3
#define LINHA_E 9
#define LINHA_D 8
#define VERMELHO 12
#define VERDE 11
#define BOTAO 13

/* Trabalhando com métodos
1 - Para andar para frente: carrinho.moverFrente();
2 - Para andar para traz:  carrinho.moverTras();
3 - Para girar para direita: carrinho.girarDireita();
4 - Para girar para esquerda: carrinho.girarEsquerda();
5 - Para parar: carrinho.parar();
6 - Para ler sensores: carrinho.lerSensores();
7 - Para atualizar os angulos do giroscopio:  giroscopio.update();
8 - Para girar para direita: giroscopio.girar(90); // 90 grus
9 - Para girar para esquerda: giroscopio.girar(-90); // -90 grus



*/

Carrinho carrinho(MOTOR_E1, MOTOR_E2, MOTOR_EV, MOTOR_D1, MOTOR_D2, MOTOR_DV, SHARP_PIN, LINHA_E, LINHA_D, VERMELHO, VERDE, BOTAO);
Giroscopio giroscopio;

// Controle do botão e estado
bool botaoPressionado = false;
int estadoFase = 0;

void setup() {
  Serial.begin(9600);
  carrinho.begin();
  giroscopio.begin();
  Serial.println("Sistema Inicializado!");
  garra.attach(10);
  garra.write(50);
  delay(2000);
}

void loop() {
  // Atualiza os valores do giroscópio
  giroscopio.update();

  // Lê sensores do carrinho
  carrinho.lerSensores();

  giroscopio.girar(90); // 90 grus

  

  // Verifica se o botão foi pressionado

  if (digitalRead(BOTAO) == LOW && !botaoPressionado) {
    delay(200); // Debounce simples
    estadoFase = (estadoFase + 1) % 3; // Alterna entre 0, 1 e 2
    botaoPressionado = true;
  } else if (digitalRead(BOTAO) == HIGH) {
    botaoPressionado = false;
  }

  // Controle das fases com switch case
  switch (estadoFase) {
    case 0: // Fase 0 - LED Vermelho
      digitalWrite(VERMELHO, HIGH);
      digitalWrite(VERDE, LOW);
      Serial.println("Fase: Vermelho");
      break;

    case 1: // Fase 1 - LED Verde
      digitalWrite(VERMELHO, LOW);
      digitalWrite(VERDE, HIGH);
      Serial.println("Fase: Verde");
      break;

    case 2: // Fase 2 - LED Amarelo (ambos ligados)
      digitalWrite(VERMELHO, HIGH);
      digitalWrite(VERDE, HIGH);
      Serial.println("Fase: Amarelo");
      break;
  }

  // Verifica se o botão está pressionado para iniciar a missão
  if (digitalRead(BOTAO) == LOW && botaoPressionado) {
    delay(1000); // Tempo para segurar o botão
    if (digitalRead(BOTAO) == LOW) {
      // Iniciar a missão e desligar os LEDs
      digitalWrite(VERMELHO, LOW);
      digitalWrite(VERDE, LOW);
      Serial.println("Missão Iniciada!");
      delay(1000); // Simula o início da missão
    }
  }

  delay(100); // Pequeno atraso para estabilizar leituras

  
}
