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
    delay(250);
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

void aumentaTempoContador() {
  if (segundos > 30) {
    if (minutos < 59) {
      minutos++;
      segundos = segundos - 30;
    }
  } else {
    segundos=segundos+30;
   }
  delay(200);
}
