unsigned long timing_upd_page;

void empty_display() {
  lcd.setCursor(0, 0); lcd.print("                    ");
  lcd.setCursor(0, 1); lcd.print("                    ");
  lcd.setCursor(0, 2); lcd.print("                    ");
  lcd.setCursor(0, 3); lcd.print("                    ");
}


void bmp_display () {
  if (bmp_clean_display == 1){
    empty_display();
    bmp_clean_display = 0;
  }
  if (millis() - timing_upd_page > 600) { // Простой таймер обновления инфы на экране
    timing_upd_page = millis();
    lcd.setCursor(0, 0); lcd.print("Barometer");
    lcd.setCursor(0, 1); lcd.print("TEMP: "); lcd.print(bme.readTemperature()); lcd.print("*C");
    lcd.setCursor(0, 2); lcd.print("PRES: "); lcd.print(bme.readPressure() / 133.3); lcd.print(" mmhg");
    lcd.setCursor(0, 3); lcd.print("HUMI: "); lcd.print(bme.readHumidity()); lcd.print(" %");
  }
}


void internet_display () {
  if (internet_clean_display == 1){
    empty_display();
    internet_clean_display = 0;
  }
  lcd.setCursor(0, 0); lcd.print(api_city);
  lcd.setCursor(0, 1); lcd.print(" "); lcd.print(api_temp.toInt()); lcd.print("*C"); lcd.print(" > "); lcd.print(api_temp_later.toInt()); lcd.print("*C");
  lcd.setCursor(0, 2); lcd.print(api_weather); lcd.print("        "); lcd.print(api_hud); lcd.print("%");
  lcd.setCursor(0, 3); lcd.print("Speed wind: "); lcd.print(api_speed_wind);
}


void time_display() {
  if (time_clean_display == 1){
    empty_display();
    time_clean_display = 0;
  }rtc_upd();
  lcd.setCursor(0, 0); lcd.print(time_); 
  lcd.setCursor(0, 1); lcd.print("      "); lcd.print(name_day_);
  lcd.setCursor(0, 2); lcd.print(" "); lcd.print(date_); 
  lcd.setCursor(0, 3); lcd.print("Adjust: "); lcd.print(adjust_time);
  
}
