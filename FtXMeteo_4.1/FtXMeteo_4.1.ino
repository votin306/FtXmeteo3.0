#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "FS.h"
#include "SPIFFS.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ErriezDS3231.h>

WiFiClient wifiClient;
LiquidCrystal_I2C lcd(0x27, 16, 4);
Adafruit_BME280 bme;
AsyncWebServer server(80);
ErriezDS3231 rtc;

const char* myFilePath = "/bmp.txt";
bool adjust_time = 1;
char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
int temp_bmp, pres_bmp, humi_bmp;
String time_, date_, name_day_;

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(4, 1);
  lcd.print("System start");
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin("ASUS_60", "3369aef687cd");
  if (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  delay(6000);
  Wire.begin();
  bme.begin(0x76);
  rtc.begin();
  rtc.setSquareWave(SquareWaveDisable);
  if (!SPIFFS.begin(true)) {
    Serial.println("Error while mounting SPIFFS");
    //  "Ошибка при монтировании SPIFFS"
    return;
  }
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/bmp.txt", "text/html");
  });
  server.serveStatic("/", SPIFFS, "/");
  server.begin();
  
  getWeatherProcess();
  upd_file();
  empty_display();
}


void loop() {
  background_process();
  
  change_page();
}
