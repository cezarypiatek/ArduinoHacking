#define MOVE_SIGNAL_PIN 7
#define OUTPUT_PIN 3
#define WORKING_HOUR_FROM 0
#define WORKING_HOUR_TO 6
#define LIGHT_DURATION 150
//#define DIAGNOSTIC

#include "RTClib.h"
RTC_DS3231 rtc;

void setup()
{
  pinMode(MOVE_SIGNAL_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
#ifdef DIAGNOSTIC
  Serial.begin(9600);
#endif

  if (! rtc.begin())
  {
#ifdef DIAGNOSTIC
    Serial.println("Couldn't find RTC");
    Serial.flush();
#endif
    abort();
  }
  else
  {
#ifdef DIAGNOSTIC
    Serial.println("RTC should working");
    Serial.flush();
#endif
  }
  if (rtc.lostPower())
  {
#ifdef DIAGNOSTIC
    Serial.println("RTC lost power, let's set the time!");
#endif
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  TestLight();
  digitalWrite(OUTPUT_PIN, LOW);
}

bool isLightOn = false;
bool previousIsMove = false;
int lightOnCounter = 0;


void LightOn()
{
  digitalWrite(OUTPUT_PIN, HIGH);
  #ifdef DIAGNOSTIC
     Serial.println("Light ON");
  #endif
}

void LightOff()
{
  digitalWrite(OUTPUT_PIN, LOW);
  #ifdef DIAGNOSTIC
     Serial.println("Light OFF");
  #endif
}
void Blink()
{
  LightOn();
  delay(2000);
  LightOff();
}

void TestLight()
{
  for (int i = 0; i < 3; i++)
  {
    Blink();
    delay(500);
  }
}

void loop() {


  bool isMove = digitalRead(MOVE_SIGNAL_PIN) == HIGH;

  if (isLightOn)
  {
    if (lightOnCounter < LIGHT_DURATION)
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
      LightOff();
    }
  }
  else
  {
    if (previousIsMove == false && isMove == true)
    {
      DateTime now = rtc.now();
#ifdef DIAGNOSTIC
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
#endif

      if (now.hour() >= WORKING_HOUR_FROM && now.hour() < WORKING_HOUR_TO)
      {
        LightOn();
        isLightOn = true;
        lightOnCounter = 0;
      }

    }
    delay(200);
  }

  previousIsMove = isMove;
}
