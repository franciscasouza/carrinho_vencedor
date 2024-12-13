#include "Carrinho.h"

Carrinho::Carrinho(int e1, int e2, int ev, int d1, int d2, int dv, int sharp, int linhaE, int linhaD, int vermelho, int verde, int botao)
    : motorE1(e1), motorE2(e2), motorEV(ev), motorD1(d1), motorD2(d2), motorDV(dv),
      pinSharp(sharp), pinLinhaE(linhaE), pinLinhaD(linhaD), pinVermelho(vermelho), pinVerde(verde), pinBotao(botao),
      tcs(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X) {
  vel = 255;
  distancia = 0;
  leituraLinhaE = leituraLinhaD = 0;
  r = g = b = c = 0;
  botaoPressionado = false;
  estadoFase = 0;
}

void Carrinho::begin() {
  pinMode(motorE1, OUTPUT);
  pinMode(motorE2, OUTPUT);
  pinMode(motorEV, OUTPUT);
  pinMode(motorD1, OUTPUT);
  pinMode(motorD2, OUTPUT);
  pinMode(motorDV, OUTPUT);
  pinMode(pinSharp, INPUT);
  pinMode(pinLinhaE, INPUT);
  pinMode(pinLinhaD, INPUT);
  pinMode(pinVermelho, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  pinMode(pinBotao, INPUT_PULLUP); // Configura o botão como entrada com pull-up

  if (!tcs.begin()) {
    Serial.println("Sensor de cor não encontrado!");
  } else {
    Serial.println("Sensor de cor inicializado!");
  }
  giroscopio.begin();
}

void Carrinho::setVelocidade(int velocidade) {
  vel = velocidade;
}

void Carrinho::moverFrente() {
  digitalWrite(motorE1, HIGH);
  digitalWrite(motorE2, LOW);
  digitalWrite(motorD1, HIGH);
  digitalWrite(motorD2, LOW);
  analogWrite(motorEV, vel);
  analogWrite(motorDV, vel);
}

void Carrinho::moverTras() {
  digitalWrite(motorE1, LOW);
  digitalWrite(motorE2, HIGH);
  digitalWrite(motorD1, LOW);
  digitalWrite(motorD2, HIGH);
  analogWrite(motorEV, vel);
  analogWrite(motorDV, vel);
}

void Carrinho::girarEsquerda() {
  digitalWrite(motorE1, HIGH);
  digitalWrite(motorE2, HIGH);
  digitalWrite(motorD1, HIGH);
  digitalWrite(motorD2, LOW);
  analogWrite(motorEV, vel);
  analogWrite(motorDV, vel);
}

void Carrinho::girarDireita() {
  digitalWrite(motorE1, HIGH);
  digitalWrite(motorE2, LOW);
  digitalWrite(motorD1, HIGH);
  digitalWrite(motorD2, HIGH);
  analogWrite(motorEV, vel);
  analogWrite(motorDV, vel);
}

void Carrinho::parar() {
  digitalWrite(motorE1, LOW);
  digitalWrite(motorE2, LOW);
  digitalWrite(motorD1, LOW);
  digitalWrite(motorD2, LOW);
  analogWrite(motorEV, 0);
  analogWrite(motorDV, 0);
}

void Carrinho::seguirLinha() {
  if (leituraLinhaE == LOW && leituraLinhaD == LOW) {
    moverFrente();
  } else if (leituraLinhaE == HIGH) {
    girarEsquerda();
  } else if (leituraLinhaD == HIGH) {
    girarDireita();
  } else {
    parar();
  }
}

// void Carrinho::verificarBotao() {
//   // Verifica se o botão foi pressionado
//   if (digitalRead(pinBotao) == LOW && !botaoPressionado) {
//     delay(200); // Debounce simples
//     estadoFase = (estadoFase + 1) % 3; // Alterna entre 0, 1 e 2
//     botaoPressionado = true;
//     Serial.print("Fase: ");
//     Serial.println(estadoFase);
//   } else if (digitalRead(pinBotao) == HIGH) {
//     botaoPressionado = false;
//   }
// }

void Carrinho::pararPorDistancia(float distanciaLimite) {
  while (true) {
    lerSensores();
    if (distancia < distanciaLimite) {
      parar();
      Serial.println("Obstáculo detectado. Carrinho parado!");
      break; // Sai do loop
    }
    seguirLinha(); // Continua a seguir linha enquanto a distância for segura
    
  }

  // Exibe a distância final no Monitor Serial
  Serial.print("Distância final: ");
  Serial.print(distancia);
  Serial.println(" cm");
}

void Carrinho::verificarCor() {
  tcs.getRawData(&r, &g, &b, &c);

  if (g > 400) {
    digitalWrite(pinVermelho, HIGH); // Liga o LED vermelho
    digitalWrite(pinVerde, HIGH);   // Liga o LED verde
    Serial.println("Cor detectada: Amarelo");
  } else {
    digitalWrite(pinVermelho, HIGH); // Liga somente o LED vermelho
    digitalWrite(pinVerde, LOW);     // Desliga o LED verde
    Serial.println("Cor detectada: Outra (não Amarelo)");
  }

  Serial.print("R: ");
  Serial.print(r);
  Serial.print(" | G: ");
  Serial.print(g);
  Serial.print(" | B: ");
  Serial.println(b);
}

void Carrinho::lerSensores() {
  int leituraSharp = analogRead(pinSharp);
  
    
    distancia = 13*pow(leituraSharp * 0.0048828125 , -1);
  
  
  

  if (distancia > 30) distancia = 30;
  if (distancia < 4) distancia = 4;

  tcs.getRawData(&r, &g, &b, &c);

  leituraLinhaE = digitalRead(pinLinhaE);
  leituraLinhaD = digitalRead(pinLinhaD);

  giroscopio.update();
}

void Carrinho::enviarDadosParaESP32() {
  String dados = "<" + String(r) + "," + String(g) + "," + String(b) + "," +
                 String(distancia) + "," + String(leituraLinhaE) + "," +
                 String(leituraLinhaD) + ">";
  Serial.println(dados);
}

void Carrinho::imprimirDadosUmaLinha() {
  Serial.print("R: ");
  Serial.print(r);
  Serial.print(" | G: ");
  Serial.print(g);
  Serial.print(" | B: ");
  Serial.print(b);
  Serial.print(" | Distância: ");
  Serial.print(distancia);
  Serial.print(" cm");
  Serial.print(" | LinhaE: ");
  Serial.print(leituraLinhaE ? "Detectada" : "Não detectada");
  Serial.print(" | LinhaD: ");
  Serial.print(leituraLinhaD ? "Detectada" : "Não detectada");
  Serial.println();
}
