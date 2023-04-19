#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int potencia = 1;               // valor inicial da potência
int minutos = 0;                // valor inicial dos minutos
int segundos = 0;               // valor inicial dos segundos
int segundos_totais = 0;        // valor total de segundos a serem contados pelo temporizador
boolean tempo_rodando = false;  // flag para saber se o temporizador está rodando
int monitor = 0;
int cursor = 0;
int tempoPisca = 0;
bool ledLigado = true;
unsigned long temporizador = 0;

void reset() {
  potencia = 1;           // valor inicial da potência
  minutos = 0;            // valor inicial dos minutos
  segundos = 0;           // valor inicial dos segundos
  segundos_totais = 0;    // valor total de segundos a serem contados pelo temporizador
  tempo_rodando = false;  // flag para saber se o temporizador está rodando
  monitor = 0;
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Microondas");
  lcd.setCursor(0, 1);
  lcd.print("Clique SELECT");
}

void loop() {
  // esperar pelo clique do botão direito para passar para a próxima tela
  if (monitor == 0) {
    if (analogRead(0) > 700 && analogRead(0) < 800) {  //Select
      monitor = 1;
      delay(300);
    }
  } else if (monitor == 1) {
    // tela de seleção de potência
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
    // tela de seleção de tempo
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
    // iniciar contagem regressiva
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
      if (analogRead(0) > 400 && analogRead(0) < 600) {  //esquerda
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

void aumentaPotencia() {
  if (potencia < 90 && cursor == 0) {
    potencia = potencia == 1 ? potencia + 9 : potencia + 10;
  } else if (potencia < 99) {
    if (cursor == 0) cursor = 1;
    potencia++;
  }
  delay(100);
}

void aumentaTempo() {
  if (minutos < 59 && cursor == 0) {
    minutos++;
  } else if (cursor == 1) {
    if (segundos == 59) {
      if (minutos < 59) {
        minutos++;
        segundos = 0;
      }
    } else {
      segundos++;
    }
  }
  delay(100);
}

void abaixaPotencia() {
  if (potencia > 10 && cursor == 0) {
    potencia = potencia - 10;
  } else if (potencia > 1) {
    potencia--;
  }
  delay(100);
}

void abaixaTempo() {
  if (minutos > 0 && cursor == 0) {
    minutos--;
  } else if (cursor == 1) {
    if (segundos > 0) {
      segundos--;
    } else if (minutos > 0) {
      minutos--;
      segundos = 59;
    }
  }
  delay(100);
}

void esquerda() {
  if (monitor > 0 && cursor == 0) {
    Serial.println("Passou");
    monitor--;
    delay(200);
    if (monitor == 0) setup();
  } else {
    cursor = 0;
    delay(200);
  }
}

void direita() {
  cursor = cursor == 0 ? 1 : 0;
  delay(100);
}

void seleciona() {
  monitor++;
  cursor = 0;
  delay(100);
}

void imprime_tempo(bool ignora) {
  lcd.setCursor(0, 1);

  if (!ledLigado && !ignora) {
    if (cursor == 0) {
      lcd.print("  :");
      if (segundos < 10) lcd.print("0");
      lcd.print(segundos);
    } else {
      if (minutos < 10) lcd.print("0");
      lcd.print(minutos);
      lcd.print(":  ");
    }
  } else {
    if (minutos < 10) lcd.print("0");
    lcd.print(minutos);
    lcd.print(":");
    if (segundos < 10) lcd.print("0");
    lcd.print(segundos);
  }
}