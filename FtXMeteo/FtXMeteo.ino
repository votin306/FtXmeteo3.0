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


#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include "DFPlayer_Mini_Mp3.h"
#include "SoftwareSerial.h"

SoftwareSerial player(11, 12);
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
DS3231  rtc(SDA, SCL);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Time  t;

unsigned long timer = 0;
unsigned long timer_button;
unsigned long time_serial;
unsigned long time_graph;
unsigned long alarm_timer;
unsigned long serial_timer;

long mmhg;
byte page;
String val_serial = "";
boolean chan = 1;
boolean chan2 = 1;
boolean chan3 = 1;
boolean chan4 = 1;
boolean chan5 = 1;
boolean alarm_state;
int alarm_h = 12;
int alarm_m = 53;
int timeout = 30;

byte x_alarm;
byte y_alarm;

void setup() {
  player.begin (9600);
  Serial.begin (9600);
  mp3_set_serial (player);  //set Serial1 for DFPlayer-mini mp3 module
  mp3_set_volume (15);
  mp3_get_tf_sum ();
  mp3_next ();
  lcd.begin();                      // initialize the lcd
  lcd.backlight();
  if (!bmp.begin()) {
    Serial.println("Барометр не найден");
    while (1) {}
  }
  rtc.begin();
  dht.begin();
  
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
    case 3: alarm();
      break;
    case 4: page = 0; chan = 1; chan2 = 1; chan4 = 1; chan3 = 1;
      break;

  }
}


void loop() {
  convert_mmhginpascal();
  page_one_char();
  change();
  alarm_chek();
  serial_data();
}
