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

void reset() {
  potencia = 1;           // valor inicial da potência
  minutos = 0;            // valor inicial dos minutos
  segundos = 0;           // valor inicial dos segundos
  segundos_totais = 0;    // valor total de segundos a serem contados pelo temporizador
  tempo_rodando = false;  // flag para saber se o temporizador está rodando
  monitor = 0;
}