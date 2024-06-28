#include <AmperkaKB.h>
#include <Ticker.h>
#include <QuadDisplay2.h>
#include <CustomStepper.h>

CustomStepper stepper(PB13, PB14, PB15, PB1, (byte[]){8, B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001}, 4075.7728395, 12, CW);
AmperkaKB KB(8, 7, 6, 5, 4, 3, 2); 
QuadDisplay qd(9);

int b = 0;
int y = 0;
int x = 0;
int alarm1 = 0;
int alarm2 = 0;
int alarm3 = 0;
int flag = 0;
int newtime = 0;
int hour = 0;
int minute = 0;
int alarm_1[] = {0,0,0,0};
int alarm_2[] = {0,0,0,0};
int alarm_3[] = {0,0,0,0};
int i[] = {0,0,0,0};
int hour1 = alarm_1[0] * 10 + alarm_1[1];
int minute1 = alarm_1[2] * 10 + alarm_1[3];
int hour2 = alarm_2[0] * 10 + alarm_2[1];
int minute2 = alarm_2[2] * 10 + alarm_2[3];
int hour3 = alarm_3[0] * 10 + alarm_3[1];
int minute3 = alarm_3[2] * 10 + alarm_3[3];  
void printDate();
void plusMinute();
Ticker DateTicker(printDate, 1000); // Тикер для обновления информации о времени на экране
Ticker timeTicker(plusMinute, 60000); // Тикер для прибавления минут

void setup() {
  Serial.begin(9600);
  stepper.setDirection(CW);
  stepper.rotateDegrees(90);
  qd.begin();
  KB.begin(KB4x3);
  DateTicker.start();
  timeTicker.start();
  attachInterrupt(4, setTime, FALLING);
  attachInterrupt(3, setTime, FALLING);
  attachInterrupt(2, setTime, FALLING);
}

void loop() {
  DateTicker.update();
  timeTicker.update();
  checkAlarm();
  flagAlarm();
  KB.read();
  if (KB.isHold()) {
    if(KB.getNum == 15) {
      flag = !flag;
      Serial.println("Succesfully");
    }
  }
    if (KB.isHold()) {
    if(KB.getNum == 1) {
      alarm1 = 1;
      Serial.println("alarm 1");
      DateTicker.pause();
      printAlarm1();
    }
  }
      if (KB.isHold()) {
    if(KB.getNum == 2) {
      alarm2 = 1;
      Serial.println("alarm 2");
      DateTicker.pause();
      printAlarm2();
    }
  }
      if (KB.isHold()) {
    if(KB.getNum == 3) {
      alarm3 = 1;
      Serial.println("alarm 3");
      DateTicker.pause();
      printAlarm3();
    }
  }
  stepper.run();
}

// счет часов
void plusHour() {
  if(hour < 23) {
  hour += 1; 
  }
  if (hour >= 23) {
    hour = 0;
  }
}

// счет минут
void plusMinute() {
  if(minute < 59) {
    minute += 1;
  }
  if(minute >= 59) {
    minute = 0;
    plusHour();
}
}

// Отображение времени на 4-х сегментный индикатор
void printDate() {
  int displayValue = hour * 100 + minute;
  qd.displayClear();
  qd.displayInt(displayValue, true);
}

void printAlarm1() {
  int displayValue = hour1 * 100 + minute1;
  qd.displayClear();
  qd.displayInt(displayValue, true);
}

void printAlarm2() {
  int displayValue = hour2 * 100 + minute2;
  qd.displayClear();
  qd.displayInt(displayValue, true);
}

void printAlarm3() {
  int displayValue = hour3 * 100 + minute3;
  qd.displayClear();
  qd.displayInt(displayValue, true);
}

// установка времени часов и будильника
void setTime() {
  KB.read();
  if (flag) {
    if (KB.justPressed()) {
      if (KB.getNum != 15 && KB.getNum != 14) {
        i[0] = i[1];
        i[1] = i[2];
        i[2] = i[3];
        i[3] = KB.getNum; 
        hour = i[0] * 10 + i[1];
        minute = i[2] * 10 + i[3];
        Serial.println("yes");
        printDate();
      }
    }
  }

  if (b) {
  if (KB.justPressed()) {
    noTone(A0);
    b = 0;
  }
}

  if (alarm1) {
    if (KB.justPressed()) {
      if (KB.getNum != 15 && KB.getNum != 14) {
        alarm_1[0] = alarm_1[1];
        alarm_1[1] = alarm_1[2];
        alarm_1[2] = alarm_1[3];
        alarm_1[3] = KB.getNum; 
        hour1 = alarm_1[0] * 10 + alarm_1[1];
        minute1 = alarm_1[2] * 10 + alarm_1[3];
        Serial.println("Будильник 1");
        printAlarm1();
      }
    }
  }

  if (alarm2) {
    if (KB.justPressed()) {
      if (KB.getNum != 15 && KB.getNum != 14) {
        alarm_2[0] = alarm_2[1];
        alarm_2[1] = alarm_2[2];
        alarm_2[2] = alarm_2[3];
        alarm_2[3] = KB.getNum; 
        hour2 = alarm_2[0] * 10 + alarm_2[1];
        minute2 = alarm_2[2] * 10 + alarm_2[3];
        Serial.println("Будильник 2");
        printAlarm2();
      }
    }
  }

  if (alarm3) {
    if (KB.justPressed()) {
      if (KB.getNum != 15 && KB.getNum != 14) {
        alarm_3[0] = alarm_3[1];
        alarm_3[1] = alarm_3[2];
        alarm_3[2] = alarm_3[3];
        alarm_3[3] = KB.getNum;
        hour3 = alarm_3[0] * 10 + alarm_3[1];
        minute3 = alarm_3[2] * 10 + alarm_3[3]; 
        Serial.println("Будильник 3");
        printAlarm3();
      }
    }
  }

  if (alarm1 || alarm2 || alarm3 || flag)
    if (KB.justPressed()) {
      if (KB.getNum == 15) {
        alarm1 = 0;
        alarm2 = 0;
        alarm3 = 0;
        flag = 0;
        Serial.println("Setting succesfully");
        DateTicker.resume();
    }
  }
}

void checkAlarm() {
  if (y == 0) {
    if ((hour1 * 100 + minute1) - (hour * 100 + minute) == 1 || (hour2 * 100 + minute2) - (hour * 100 + minute) == 1 || (hour3 * 100 + minute3) - (hour * 100 + minute) == 1) {
      if (hour * 100 + minute != 0) {
      y = 5;
      Serial.println("rotate");
      rotato();
      }
    }
  }
  if (x == 0){
    if (hour * 100 + minute == hour1 * 100 + minute1 || hour * 100 + minute == hour2 * 100 + minute2 || hour * 100 + minute == hour3 * 100 + minute3) {
      if (hour * 100 + minute != 0) {
      x = 5;
      tone(A0, 100);
      b = 1;
      Serial.println("ALARM!!");
      }
    }
  }
}

void flagAlarm() {
  if (x == 5) {
    if (hour * 100 + minute - hour1 * 100 + minute1 == 1) {
      x = 0;
    }
    if (hour * 100 + minute - hour2 * 100 + minute2 == 1){
      x = 0;
    }
    if (hour * 100 + minute - hour3 * 100 + minute3 == 1) {
      x = 0;
    }
  }
    if (y == 5) {
    if (hour * 100 + minute - hour1 * 100 + minute1 == 1) {
      y = 0;
    }
    if (hour * 100 + minute - hour2 * 100 + minute2 == 1){
      y = 0;
    }
    if (hour * 100 + minute - hour3 * 100 + minute3 == 1) {
      y = 0;
    }
  }
}

void rotato() {
  stepper.setDirection(CW);
  stepper.rotateDegrees(90);
}
