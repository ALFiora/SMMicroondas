#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#include "Variaveis.h"
#include "Botoes.h"
#include "Utilidades.h"

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Microondas");
  lcd.setCursor(0, 1);
  lcd.print("Clique SELECT");
}

void loop() {
  //Esperar pelo clique do botão direito para passar para a próxima tela
  if (monitor == 0) {
    if (analogRead(0) > 700 && analogRead(0) < 800) {
      monitor = 1;
      delay(300);
    }
  } else if (monitor == 1) {
    //Tela de seleção de potência
    lcd.clear();
    lcd.print("Potencia");
    lcd.setCursor(0, 1);
    if (tempoPisca == 5) {
      ledLigado = !ledLigado;
      tempoPisca = 0;
    }
    if (!ledLigado) {
      if (cursor == 0) {
        lcd.print(" ");
        lcd.print(potencia < 10 ? potencia : potencia % 10);
      } else {
        lcd.print(potencia < 10 ? 0 : potencia / 10);
        lcd.print(" ");
      }
    } else {
      if (potencia < 10) lcd.print("0");
      lcd.print(potencia);
    }
    if (analogRead(0) < 80) {
      direita();
    } else if (analogRead(0) < 200) {
      aumentaPotencia();
    } else if (analogRead(0) < 400) {
      abaixaPotencia();
    } else if (analogRead(0) < 600) {
      esquerda();
    } else if (analogRead(0) < 800) {
      seleciona();
    }
    delay(100);  //Se não colocar delay, a tela fica piscando
    tempoPisca++;
  } else if (monitor == 2) {
    //Tela de seleção de tempo
    lcd.clear();
    lcd.print("Tempo");
    if (tempoPisca == 5) {
      ledLigado = !ledLigado;
      tempoPisca = 0;
    }
    imprime_tempo(false);
    if (analogRead(0) < 80) {
      direita();
    } else if (analogRead(0) < 200) {
      aumentaTempo();
    } else if (analogRead(0) < 400) {
      abaixaTempo();
    } else if (analogRead(0) < 600) {
      esquerda();
    } else if (analogRead(0) < 800) {
      seleciona();
    }
    delay(100);  //Se não colocar delay, a tela fica piscando
    tempoPisca++;
  } else if (monitor == 3) {
    //Inicia a contagem regressiva
    temporizador = millis();
    lcd.clear();
    lcd.print("Contando");
    segundos_totais = minutos * 60 + segundos;
    tempo_rodando = true;
    while (tempo_rodando) {
      imprime_tempo(true);
      if ((millis() - temporizador) >= 1000) {
        segundos_totais--;
        temporizador = millis();
      }
      minutos = segundos_totais / 60;
      segundos = segundos_totais % 60;
      if (segundos_totais < 0) {
        tempo_rodando = false;
        monitor++;
      }
      if (analogRead(0) > 400 && analogRead(0) < 600) {
        esquerda();
        tempo_rodando = false;
      }
    }
  } else if (monitor == 4) {
    lcd.clear();
    lcd.print("Fim");
    reset();
    delay(2000);
    lcd.clear();
    setup();
  }
}