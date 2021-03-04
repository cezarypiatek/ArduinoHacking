#define MOVE_SIGNAL_PIN 7
#define OUTPUT_PIN 3

#include "RTClib.h"
RTC_DS3231 rtc;

void setup()
{
  pinMode(MOVE_SIGNAL_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  Serial.begin(9600);
  
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  } 
  else 
  {
    Serial.println("RTC should workingC");
    Serial.flush();
  }
  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  digitalWrite(OUTPUT_PIN, LOW);
}

bool isLightOn = false;
bool previousIsMove = false;
int lightOnCounter = 0;

void loop() {
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  bool isMove = digitalRead(MOVE_SIGNAL_PIN) == HIGH;

  if (isLightOn)
  {
    if (lightOnCounter < 150)
    {
      if (lightOnCounter > 130 && previousIsMove == false && isMove == true)
      {
        lightOnCounter -= 20;
      }
      else {
        lightOnCounter++;
      }
      delay(1000);
    }
    else
    {
      isLightOn = false;
      digitalWrite(OUTPUT_PIN, LOW);
    }
  }
  else
  {
    if (previousIsMove == false && isMove == true && now.hour() < 6)
    {
      digitalWrite(OUTPUT_PIN, HIGH);
      isLightOn = true;
      lightOnCounter = 0;
    }
    delay(200);
  }

  previousIsMove = isMove;
}
