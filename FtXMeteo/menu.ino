void serial_data() {
  parsingStream((int*)&alarm);               // функция парсинга, парсит в массив!
  if (dataReady()) {
    Serial.println("Короче, я поставил будильник, и начинаю его настраивать");
    Serial.print("На "); Serial.print(alarm[0]); Serial.println(" часов");
    Serial.print("И на "); Serial.print(alarm[1]); Serial.println(" митуты");
    Serial.print("А продолжительность "); Serial.print(alarm[2]); Serial.println(" секунд");
  }
  if (millis() - serial_timer_tx >= 10000) {
    //serial_timer_tx = millis();
    //float h = dht.readHumidity();
    //float t = dht.readTemperature();
    //float f = dht.readTemperature(true);
    //Serial.print("ТЕМПЕРАТУРА(барометр) = "); Serial.println (bmp.readTemperature());
    //Serial.print("ДАВЛЕНИЕ = "); Serial.println(bmp.readPressure());
    //Serial.print("ДавНа Уровне Моря = "); Serial.println(bmp.readSealevelPressure());
    //Serial.print("ТЕМПЕРАТУРА(DHT22) = "); Serial.println(t);
    //Serial.print("ВЛАЖНОСТЬ = "); Serial.println(h);

  }
}
void temp_press_diplay() {
  if (chan4 == 1) {
    lcd.setCursor(0, 0); lcd.print("                    ");
    lcd.setCursor(0, 1); lcd.print("                    ");
    lcd.setCursor(0, 2); lcd.print("                    ");
    lcd.setCursor(0, 3); lcd.print("                    ");
    chan4 = 0;
  }
  if (millis() - timer_bmp >= upd_disp) {
    timer_bmp = millis();
    lcd.setCursor(0, 0); lcd.print("BMP180/085");
    lcd.setCursor(0, 1); lcd.print("TEMPERATURE");
    lcd.setCursor(12, 1); lcd.print(bmp.readTemperature());
    lcd.setCursor(17, 1); lcd.print("C");
    if (bmp.readTemperature() < porog_temp) {
      lcd.setCursor(19, 1);
      lcd.print("\2");
    }
    if (bmp.readTemperature() > porog_temp) {
      lcd.setCursor(19, 1);
      lcd.print("\1");
    }

    if (type_press == 1) {
      lcd.setCursor(0, 2); lcd.print("PRESSURE");
      lcd.setCursor(12, 2); lcd.print(bmp.readPressure());
      lcd.setCursor(17, 2); lcd.print("P");
      lcd.setCursor(0, 3); lcd.print("ALTITUDE");
      lcd.setCursor(12, 3); lcd.print(bmp.readAltitude());
      lcd.setCursor(19, 3); lcd.print("m");
    }
    if (type_press == 0) {
      lcd.setCursor(0, 2); lcd.print("PRESSURE");
      lcd.setCursor(12, 2); lcd.print(mmhg);
      lcd.setCursor(17, 2); lcd.print("mHg");
      lcd.setCursor(0, 3); lcd.print("ALTITUDE");
      lcd.setCursor(12, 3); lcd.print(bmp.readAltitude());
      lcd.setCursor(19, 3); lcd.print("m");
    }
  }
}

void convert_mmhginpascal() {
  mmhg = bmp.readPressure() / 133;
}

void times() {
  t = rtc.getTime();
  if (chan == 1) {
    lcd.setCursor(0, 0); lcd.print("                    ");
    lcd.setCursor(0, 1); lcd.print("                    ");
    lcd.setCursor(0, 2); lcd.print("                    ");
    lcd.setCursor(0, 3); lcd.print("                    ");
    chan = 0;
  }
  lcd.setCursor(0, 0); lcd.print("TIME");
  lcd.setCursor(0, 1); lcd.print("DAY");
  lcd.setCursor(0, 2); lcd.print("TIME");
  lcd.setCursor(0, 3); lcd.print("DATE");
  lcd.setCursor(5, 1); lcd.print(rtc.getDOWStr());
  lcd.setCursor(5, 3); lcd.print(rtc.getDateStr());
  lcd.setCursor(5, 2); lcd.print(rtc.getTimeStr());
}

void DHTs() {
  if (chan2 == 1) {
    lcd.setCursor(0, 0); lcd.print("                    ");
    lcd.setCursor(0, 1); lcd.print("                    ");
    lcd.setCursor(0, 2); lcd.print("                    ");
    lcd.setCursor(0, 3); lcd.print("                    ");
    chan2 = 0;
  }
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  lcd.setCursor(0, 0); lcd.print("DHT22");
  lcd.setCursor(0, 1); lcd.print("HUMIDITU");
  lcd.setCursor(0, 2); lcd.print("TEMPERATURE");
  lcd.setCursor(0, 3); lcd.print("TEMPERATURE");
  lcd.setCursor(12, 1); lcd.print(h);
  lcd.setCursor(12, 2); lcd.print(t);
  lcd.setCursor(12, 3); lcd.print(f);
  lcd.setCursor(18, 1); lcd.print("%");
  lcd.setCursor(18, 2); lcd.print("C");
  lcd.setCursor(18, 3); lcd.print("F");
}

void disp_alarm() {
  if (chan3 == 1) {
    lcd.setCursor(0, 0); lcd.print("                    ");
    lcd.setCursor(0, 1); lcd.print("                    ");
    lcd.setCursor(0, 2); lcd.print("                    ");
    lcd.setCursor(0, 3); lcd.print("                    ");
    chan3 = 0;
  }
  lcd.setCursor(0, 0); lcd.print("Alarm");
  lcd.setCursor(12, 0); lcd.print(rtc.getTimeStr());
  lcd.setCursor(0, 1); lcd.print("TIME = ");
  lcd.setCursor(7, 1); lcd.print(alarm[0]);
  lcd.setCursor(9, 1); lcd.print(":");
  lcd.setCursor(10, 1); lcd.print(alarm[1]);
  lcd.setCursor(0, 2); lcd.print("TIMEOUT = ");
  lcd.setCursor(10, 2); lcd.print(alarm[2]);
}

void check_alarm() {
  t = rtc.getTime();
  if (t.hour == alarm[0] and t.min == alarm[1] and t.sec == 0) {
    mp3_prev();
  }
  if (t.hour == alarm[0] and t.min == alarm[1] and t.sec == alarm[2]) {
    mp3_pause();
  }
}
