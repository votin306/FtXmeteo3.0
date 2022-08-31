String api_city, api_temp, api_weather, api_hud, api_speed_wind, api_temp_later;
void getWeatherProcess() {
  if ((WiFi.status() == WL_CONNECTED)) {  //Check the current connection status
    // создаем объект для работы с HTTP
    HTTPClient http;
    // подключаемся к веб-странице OpenWeatherMap с указанными параметрами
    http.begin(wifiClient, "http://api.openweathermap.org/data/2.5/forecast?q=Chusovoy,567434&APPID=4b79ee3cc5f9a79b300fad9af01428fd&mode=json&units=metric&cnt=2");
    int httpCode = http.GET();  // Делаем запрос
    if (httpCode > 0) {         // проверяем ответ сервера
      // выводим ответ сервера
      String payload = http.getString();
      const size_t bufferSize = 2 * JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + 4 * JSON_OBJECT_SIZE(1) + 3 * JSON_OBJECT_SIZE(2) + 3 * JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 2 * JSON_OBJECT_SIZE(7) + 2 * JSON_OBJECT_SIZE(8) + 720;
      DynamicJsonBuffer jsonBuffer(bufferSize);

      JsonObject& root = jsonBuffer.parseObject(payload);
      if (!root.success()) {
        Serial.println("parseObject() failed");
        return;
      }
      JsonArray& list = root["list"];
      JsonObject& nowT = list[0];
      JsonObject& later = list[1];

      String city = root["city"]["name"];
      api_city = city;

      String temp = nowT["main"]["temp"];
      api_temp = temp;

      String temp_later = later["main"]["temp"];
      api_temp_later = temp_later;

      String weather = nowT["weather"][0]["main"];
      api_weather = weather;

      String hud = nowT["main"]["humidity"];
      api_hud = hud;

      String speed_wind = nowT["wind"]["speed"];
      api_speed_wind = speed_wind;
    } else if (httpCode == 404) {
      Serial.println("Город не найден");
    } else {
      Serial.println("Не удалось получить данные");
    }
    http.end();  // освобождаем ресурсы микроконтроллера
  }
}

bool bmp_clean_display = 1;
bool internet_clean_display = 1;
bool time_clean_display = 1;
byte page;
unsigned long timer_button;

void change_page() {
  if (millis() - timer_button >= 1000) {
    timer_button = millis();
    if (digitalRead(13)) {
      page++;
    }
  }
  switch (page) {
    case 0: bmp_display();
      break;
    case 1: internet_display ();
      break;
    case 2: time_display ();
      break;
    case 3: page = 0; bmp_clean_display = 1; internet_clean_display = 1; time_clean_display = 1;
      break;
  }
}

void upd_file() {
  rtc_upd();
  String stroka;
  stroka += date_;
  stroka += " - ";
  stroka += time_;
  stroka += " --> Temp:";
  stroka += bme.readTemperature();
  stroka += "*C    Pressure:";
  stroka += bme.readPressure() / 133.3;
  stroka += "mmHg    Humidity:";
  stroka += bme.readHumidity();
  stroka += "%";
  File myFile = SPIFFS.open(myFilePath, FILE_APPEND);
  myFile.println(stroka);
  myFile.close();
}

unsigned long timer_file_upd;
void background_process(){
  if (millis() - timer_file_upd >= 3600000) {
    timer_file_upd = millis();
    upd_file();
    empty_display();
  }
}

void rtc_upd(){
  uint8_t hour; uint8_t min; uint8_t sec; uint8_t mday; uint8_t mon; uint16_t year; uint8_t wday;
  rtc.getDateTime(&hour, &min, &sec, &mday, &mon, &year, &wday);
  
  if (hour < 10) {time_ = "0";}  time_ += hour;time_ += ":";
  if (min < 10) {time_ += "0";}  time_ +=min;  time_ += ":";
  if (sec < 10) {time_ += "0";}  time_ +=sec;  
  
  if (mday < 10) {date_ = "0";} date_ += mday;date_ += ".";
  if (mon < 10) {date_ += "0";} date_ += mon; date_ += "."; date_ += year;
  name_day_ = daysOfTheWeek[wday];
}
