#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int potencia = 1; // valor inicial da potência
int minutos = 0; // valor inicial dos minutos
int segundos = 0; // valor inicial dos segundos
int segundos_totais = 0; // valor total de segundos a serem contados pelo timer
boolean timer_running = false; // flag para saber se o timer está rodando
int screen = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Microondas");
  lcd.setCursor(0, 1);
  lcd.print("Clique RIGHT");
}

void loop() {
  // esperar pelo clique do botão direito para passar para a próxima tela
  if(screen == 0) {
    if (analogRead(0) < 80) {
      screen = 1;
      delay(500);
    }
  } else if (screen == 1) {
    // tela de seleção de potência
    lcd.clear();
    lcd.print("Potencia");
    lcd.setCursor(0, 1);
    lcd.print(potencia);
    if (analogRead(0) < 80) {
      screen = 2;
      delay(500);
    } else if (analogRead(0) < 200) {
      upPotencia();
    } else if (analogRead(0) < 400) {
      downPotencia();
    } else if (analogRead(0) < 600) {
      left();
      delay(300);
    }
    delay(100);
  } else if (screen == 2) {
    // tela de seleção de tempo
    lcd.clear();
    lcd.print("Tempo");
    print_time();
    if (analogRead(0) < 80) {
      screen = 3;
      delay(500);
    } else if (analogRead(0) < 200) {
      upTempo();
      delay(300);
    } else if (analogRead(0) < 400) {
      downTempo();
    } else if (analogRead(0) < 600) {
      left();
      delay(300);
    }
    delay(100);
  } else if (screen == 3) {
    // iniciar contagem regressiva
    lcd.clear();
    lcd.print("Contando");
    segundos_totais = minutos * 60 + segundos;
    timer_running = true;
    while (timer_running) {
      lcd.setCursor(0, 1);
      delay(1000);
      segundos_totais--;
      minutos = segundos_totais/60;
      segundos = segundos_totais%60;
      Serial.println(segundos);
      if(segundos == 9) {
        lcd.clear();
        lcd.print("Contando");
      }
      print_time();
      if (segundos_totais < 0) {
        timer_running = false;
      }
    }
    lcd.clear();
    lcd.print("Fim");
    delay(2000);
    lcd.clear();
    screen = 1;
    setup();
  }
  
}


void upPotencia() {
  if (potencia < 99) {
    potencia++;
    lcd.setCursor(0, 1);
    lcd.print(potencia);
  } 
}

void upTempo() {
  if (minutos < 60) {
    minutos++;
    print_time();
  } 
}

void downPotencia() {
  if (potencia > 1) {
    potencia--;
    lcd.setCursor(0, 1);
    lcd.print(potencia);
  }
}

void downTempo() {
  if (minutos > 0) {
    minutos--;
    print_time();
  }
}

void left() {
  if(screen > 0) {
    screen--;
  }
}

void print_time() {
  lcd.setCursor(0, 1);
  lcd.print(minutos);
  lcd.print(":");
  lcd.print(segundos);
}