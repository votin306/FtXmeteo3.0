void bmp_page() {
  sumbols();
  if (bmp_page_upd == 1) {
    lcd.setCursor(0, 0); lcd.print("                    ");
    lcd.setCursor(0, 1); lcd.print("                    ");
    lcd.setCursor(0, 2); lcd.print("                    ");
    lcd.setCursor(0, 3); lcd.print("                    ");
    bmp_page_upd = 0;
  }
  lcd.setCursor(0, 0); lcd.print("Barometer");
  t = rtc.getTime();
  if (PRESS_TYPE == 1) {
    if (millis() - timer_bmp_paskal >= upd_disp) {
      lcd.setCursor(0, 3); lcd.print("PR:"); lcd.print(bmp.readPressure() / 133); lcd.print(" mmhg");
      lcd.setCursor(0, 2); lcd.print("TM:"); lcd.print(bmp.readTemperature()); lcd.print(" C");
      lcd.setCursor(0, 1); lcd.print("AT:"); lcd.print(bmp.readAltitude()); lcd.print(" m");
      lcd.setCursor(11, 1); lcd.print("SP:"); lcd.print(bmp.readSealevelPressure(115) / 133); lcd.print("mHg");
      if (porog_temp < bmp.readTemperature()) {
        lcd.setCursor(11, 2);
        lcd.write(3);
      } else {
        lcd.setCursor(11, 2);
        lcd.write(4);
      }
      timer_bmp_paskal = millis();
    }
  }

  if (PRESS_TYPE == 2) {
    if (millis() - timer_bmp_paskal >= upd_disp) {
      lcd.setCursor(0, 3); lcd.print("PR:"); lcd.print(bmp.readPressure()); lcd.print(" p");
      lcd.setCursor(0, 2); lcd.print("TM:"); lcd.print(bmp.readTemperature()); lcd.print(" C");
      lcd.setCursor(0, 1); lcd.print("AT:"); lcd.print(bmp.readAltitude()); lcd.print(" m");
      lcd.setCursor(11, 1); lcd.print("SP:"); lcd.print(bmp.readSealevelPressure(115) / 133); lcd.print("mHg");
      timer_bmp_paskal = millis();
    }
  }
  lcd.setCursor(15, 0); lcd.print(t.hour); lcd.print(":"); lcd.print(t.min);
  if (bmp.readPressure() / 133 < 750) {
    lcd.setCursor(13, 3); lcd.print("W:Rain");
  }
  if (751 < bmp.readPressure() / 133 < 770) {
    lcd.setCursor(13, 3); lcd.print("W:Norm");
  }
  if (bmp.readPressure() / 133 > 771) {
    lcd.setCursor(13, 3); lcd.print("W:Sun");
  }
}

void dht_page() {
  if (dth_page_upd == 1) {
    lcd.setCursor(0, 0); lcd.print("                    ");
    lcd.setCursor(0, 1); lcd.print("                    ");
    lcd.setCursor(0, 2); lcd.print("                    ");
    lcd.setCursor(0, 3); lcd.print("                    ");
    dth_page_upd = 0;
  }

  lcd.setCursor(0, 0); lcd.print("\xFF");
}
