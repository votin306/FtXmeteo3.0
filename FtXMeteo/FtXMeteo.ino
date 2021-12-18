//----------------------FtXMeteo 3.0------------------
//-----------ПРОСТАЯ МЕТЕОСТАНЦИЯ НА ARDUINO----------
//------------Разрабатывал--FtX (Ft Xivan)------------
//-------------Вопросы asuste035@gmail.com------------
//САЙТ-https://sites.google.com/view/ft-xivan-project/
//------Если хотие настроить то вам сюди (вниз)-------

//*****************НАСТРОЙКИ*****************
#define DHTTYPE DHT22    // Тип модуля, есть DHT11 а есть DHT22
#define DHTPIN 2         // Куда подключен модуль DHT
#define BTNPIN 3         // Куда подключена кнопка
#define BLUEON 1         // Блютуз передача
#define PRESS_TYPE 1     // Вид давления. 1 - Mmhg(миллиметры ртутного столба). 0 - Паскали

float porog_temp = 21.5; // При какой температуре измнит смайлик
int upd_disp =  1500;    // Обновление данных на экране
int change_page =  2000; // Задержка между страницами
boolean type_press = 0;  // Тип давления, 0 mmhg или 1 в паскалях
int serial_send_pakage = 100000; // Задежка отпаравка данных на телефон

#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include "SoftwareSerial.h"
#include <DFPlayer_Mini_Mp3.h>

DHT dht(DHTPIN, DHTTYPE);                    //Температурный датчик
Adafruit_BMP085 bmp;                         //Барометр
DS3231  rtc(SDA, SCL);                       //Датчик реального времени
LiquidCrystal_I2C lcd(0x27, 20, 4);          //Дисплей. 1 значение: адрес, 2 значение: ширина, 3 значение: высота
Time  t;                                     //СОЗДАНИЕ КЛАССА хранящее время отдельно
SoftwareSerial player(10, 11);               //Плеер
SoftwareSerial hc_06(8, 9);                  //БлютуХ. rx 9. tx 8

//**************ТАЙМЕРЫ**************
unsigned long timer_bmp_paskal;
unsigned long timer_button;
unsigned long time_serial;
unsigned long time_graph;
unsigned long alarm_timer;
unsigned long serial_timer_tx;
unsigned long test_timer;

//**************ПЕРЕМЕННЫЕ КАКИЕ-ТО**************
long mmhg; //Давление
byte page; //СМЕНА СТРАНИЦ
String val_serial = ""; //Данные с порта
boolean recievedFlag; //Принятие данных в порт? приняли
boolean bmp_page_upd = 1;  // переменная для 2 страницы(Время) чтобы выполнить сброс предыдущей страницы
boolean time_page_upd = 1; // переменная для 3 страницы(DHT22) чтобы выполнить сброс предыдущей страницы
boolean dth_page_upd = 1; // переменная для 4 страницы чтобы выполнить сброс предыдущей страницы
boolean alarm_page_upd = 1; // переменная для 1 страницы(барометр)чтобы выполнить сброс предыдущей страницы
boolean alarm_state;       //Состояние будильника
boolean test_on = 0;       //Тест идет или нет

void(* resetFunc) (void) = 0;

void setup() {
  Serial.begin (9600);
  lcd.begin();
  lcd.backlight();

  bmp.begin();
  rtc.begin();
  dht.begin();
  player.begin (9600);
  hc_06.begin(9600);

  //rtc.setDOW(FRIDAY);
  //rtc.setTime(22, 22, 0);
  //rtc.setDate(11, 12, 2021);

  mp3_set_serial (player);  //set softwareSerial for DFPlayer-mini mp3 module
  mp3_set_volume (15);

  if (digitalRead(BTNPIN)) {
    lcd.setCursor(0, 0); lcd.print("TECT Meteo");
    lcd.setCursor(12, 0); lcd.print("."); delay(500); lcd.setCursor(13, 0); lcd.print("."); delay(500); lcd.setCursor(14, 0); lcd.print("."); delay(500);
    if (isnan(dht.readHumidity())) {
      lcd.setCursor(0, 1);
      lcd.print("DHT22 not found :( ");
      resetFunc();
    } else {
      lcd.setCursor(0, 1);
      lcd.print("DHT22 found :)");
    }
    if (!bmp.begin()) {
      lcd.setCursor(0, 2);
      lcd.print("BMPxxx not found :(");
      resetFunc();
    } else {
      lcd.setCursor(0, 2);
      lcd.print("BMPxxx found :)");
    }
    if (isnan(rtc.getTemp())) {
      lcd.setCursor(0, 3);
      lcd.print("RTC not found :(");
      resetFunc();
    } else {
      lcd.setCursor(0, 3);
      lcd.print("RTC found :)");
    }
    delay(5000);
    test_on == 0;
    page = 1;
  }
}

void change() {
  if (test_on == 0) {
    if (millis() - timer_button >= change_page) {
      if (digitalRead(BTNPIN)) {
        page++;
      } timer_button = millis();
    }
    switch (page) {
      case 0: bmp_page();
        break;
      case 1: dht_page();
        break;
      case 2: page = 0; dth_page_upd = 1; bmp_page_upd = 1;
        break;
    }
  }
}

void loop() {
  change();
}
/*
        ┏---------┓      |        /|     ┏---------┓     |-----------      |         |
        |         |      |       / |     |         |     |                 |         |
        |         |      |      /  |     |         |     |                 |         |
        |         |      |     /   |     |         |     |                 |         |
        |         |      |    /    |     |         |     |------           |         |
        |         |      |   /     |     |         |     |                 |         |
        |         |      |  /      |     |         |     |                 |---------|--┓
        |         |      | /       |     |         |     |                              |
        |         |      |/        |     |         |     |-----------                   |

                                                                                   BY Fun community
*/
