#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int potencia = 1; // valor inicial da potência
int minutos = 0; // valor inicial dos minutos
int segundos = 0; // valor inicial dos segundos
int segundos_totais = 0; // valor total de segundos a serem contados pelo timer
boolean timer_running = false; // flag para saber se o timer está rodando
int screen = 0;
int cursor = 0; 
int tempoPisca = 0;
bool ledLigado = true;
unsigned long timer = 0;

void reset() {
  potencia = 1; // valor inicial da potência
  minutos = 0; // valor inicial dos minutos
  segundos = 0; // valor inicial dos segundos
  segundos_totais = 0; // valor total de segundos a serem contados pelo timer
  timer_running = false; // flag para saber se o timer está rodando
  screen = 0;
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
  if(screen == 0) {
    if (analogRead(0) > 700 && analogRead(0) < 800) { //Select
      screen = 1;
      delay(300);
    }
  } else if (screen == 1) {
    // tela de seleção de potência
    lcd.clear();
    lcd.print("Potencia");
    lcd.setCursor(0, 1);
    if (tempoPisca == 5) {
      ledLigado = !ledLigado;
      tempoPisca = 0;
    }
    if(!ledLigado) {
      if (cursor == 0) {
        lcd.print(" ");
        lcd.print(potencia < 10 ? potencia : potencia%10);
      } else {
        lcd.print(potencia < 10 ? 0 : potencia/10);
        lcd.print(" ");
      }
    } else {
      if (potencia < 10) lcd.print("0");
      lcd.print(potencia);
    }
    if (analogRead(0) < 80) {
      right();
    } else if (analogRead(0) < 200) {
      upPotencia();
    } else if (analogRead(0) < 400) {
      downPotencia();
    } else if (analogRead(0) < 600) {
      left();
    } else if (analogRead(0) < 800) {
      select();
    }
    delay(100); //Se não colocar delay, a tela fica piscando
    tempoPisca++;
  } else if (screen == 2) {
    // tela de seleção de tempo
    lcd.clear();
    lcd.print("Tempo");
    if (tempoPisca == 5) {
      ledLigado = !ledLigado;
      tempoPisca = 0;
    }
    print_time(false);
    if (analogRead(0) < 80) {
      right();
    } else if (analogRead(0) < 200) {
      upTempo();
    } else if (analogRead(0) < 400) {
      downTempo();
    } else if (analogRead(0) < 600) {
      left();
    } else if (analogRead(0) < 800) {
      select();
    }
    delay(100); //Se não colocar delay, a tela fica piscando
    tempoPisca++;
  } else if (screen == 3) {
    // iniciar contagem regressiva
    timer = millis();
    lcd.clear();
    lcd.print("Contando");
    segundos_totais = minutos * 60 + segundos;
    timer_running = true;
    while (timer_running) {
      print_time(true);
      if ((millis() - timer) >= 1000) {
        segundos_totais--;
        timer = millis();
      }
      minutos = segundos_totais/60;
      segundos = segundos_totais%60;
      if (segundos_totais < 0) {
        timer_running = false;
        screen++;
      }
      if (analogRead(0) > 400 && analogRead(0) < 600) { //Left
        left();
        timer_running = false;
      }
    }
  } else if (screen == 4) {
    lcd.clear();
    lcd.print("Fim");
    reset();
    delay(2000);
    lcd.clear();
    setup();
  }
  
}


void upPotencia() {
  if (potencia < 90 && cursor == 0) {
    potencia = potencia == 1 ? potencia+9 : potencia+10;
  } else if (potencia < 99) {
    if (cursor == 0) cursor = 1;
    potencia++;
  }
  delay(100);
}

void upTempo() {
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

void downPotencia() {
  if (potencia > 10 && cursor == 0) {
    potencia= potencia - 10;
  } else if (potencia > 1) {
    potencia--;
  }
  delay(100);
}

void downTempo() {
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

void left() {
  if(screen > 0 && cursor == 0) {
    Serial.println("Passou");
    screen--;
    delay(200);
    if (screen == 0) setup();
  } else {
    cursor = 0;
    delay(200);
  }
}

void right() {
  cursor = cursor == 0 ? 1 : 0;
  delay(100);
}

void select () {
  screen++;
  cursor = 0;
  delay(100);
}

void print_time(bool ignore) {
  lcd.setCursor(0, 1);

  if(!ledLigado && !ignore) {
    if (cursor == 0) {
      lcd.print("  :");
      if(segundos < 10) lcd.print("0");
      lcd.print(segundos);
    } else {
      if(minutos < 10) lcd.print("0");
      lcd.print(minutos);
      lcd.print(":  ");
    }
  } else {
    if(minutos < 10) lcd.print("0");
    lcd.print(minutos);
    lcd.print(":");
    if(segundos < 10) lcd.print("0");
    lcd.print(segundos);
  }


}