//----------------------FtXMeteo 3.0------------------
//-----------ПРОСТАЯ МЕТЕОСТАНЦИЯ НА ARDUINO----------
//------------Разрабатывал--FtX (Ft Xivan)------------
//-------------Вопросы asuste035@gmail.com------------
//САЙТ-https://sites.google.com/view/ft-xivan-project/
//------Если хотие настроить то вам сюди (вниз)-------

//*****************НАСТРОЙКИ*****************
#define DHTTYPE DHT22
#define DHTPIN 2

float porog_temp = 26.5;
int upd_disp =  1500;
int change_page =  2000;
boolean type_press = 0;

int alarm[3]; // массив численных значений после парсинга

#include "GParsingStream.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include "SoftwareSerial.h"
#include <DFPlayer_Mini_Mp3.h>

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
DS3231  rtc(SDA, SCL);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Time  t;
SoftwareSerial player(10, 11);
SoftwareSerial mySerial(10, 11);

unsigned long timer_bmp;
unsigned long timer_button;
unsigned long time_serial;
unsigned long time_graph;
unsigned long alarm_timer;
unsigned long serial_timer_tx;
unsigned long serial_timer_rx;

long mmhg;
byte page;
String val_serial = "";
boolean recievedFlag;
boolean chan = 1; // переменная для 2 страницы(Время) чтобы выполнить сброс предыдущей страницы
boolean chan2 = 1; // переменная для 3 страницы(DHT22) чтобы выполнить сброс предыдущей страницы
boolean chan3 = 1; // переменная для 4 страницы чтобы выполнить сброс предыдущей страницы
boolean chan4 = 1; // переменная для 1 страницы(барометр)чтобы выполнить сброс предыдущей страницы
boolean chan5 = 1; 
boolean alarm_state;
int timeout;

byte x_alarm;
byte y_alarm;

void setup() {
  Serial.begin (9600);
  lcd.begin();                      // initialize the lcd
  lcd.backlight();
  if (!bmp.begin()) {
    Serial.println("Барометр не найден");
    while (1) {}
  }
  rtc.begin();
  dht.begin();
  player.begin (9600);
  mp3_set_serial (player);  //set softwareSerial for DFPlayer-mini mp3 module 
  mp3_set_volume (15);
  
}

void change() {
  if (millis() - timer_button >= 1000) {
    timer_button = millis();
    if (digitalRead(3)) {
      page++;
    }
  }
  switch (page) {
    case 0: page_one_char(); temp_press_diplay();
      break;
    case 1: times();
      break;
    case 2: DHTs();
      break;
    case 3: disp_alarm();
      break;
    case 4: page = 0; chan = 1; chan2 = 1; chan4 = 1; chan3 = 1;chan5 = 1;
      break;

  }
}


void loop() {
  convert_mmhginpascal();
  page_one_char();
  change();
  serial_data();
  check_alarm();
}
