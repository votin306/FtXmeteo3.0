/* Вспомогательная программа для FtXmeteo
 *  Нужна для установки времени на DS3231 
 *  Для рабты нужна бибилотека она есть тут https://github.com/votin306/FtXmeteo3.0
 *  Вскоре сам весь процесс установки времени будет встроен в саму FtXmeteo
 */

#include <DS3231.h>
DS3231  rtc(SDA, SCL);
Time  t;

void setup()
{
  Serial.begin(115200);
  rtc.begin();
  
  rtc.setDOW(WEDNESDAY);     // на английском день недели, кто ничего не понимает вам сюда https://puzzle-english.com/wp-content/uploads/w11.png
  rtc.setTime(12, 0, 0);     // 12:00:00 (24hr format)
  rtc.setDate(1, 1, 2014);   // 1 это месяц, 2 это день, 3 это год
}
