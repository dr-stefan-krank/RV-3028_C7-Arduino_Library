#include <RV-3028-C7.h>

RV3028 rtc;
void RTCSleep(int hours, int minutes);

void setup()
{
    Serial.begin(115200); // Start the Serial communication to send messages to the computer
    delay(1000);
    Serial.println("I'm here");

    Wire.begin();

    if (rtc.begin(Wire, true, true, true, false) == false)
    {
        Serial.println("Problem initializing the rtc.");
    }
    else
    {
        if (!rtc.readPORIE())
        {
            Serial.println("Power On Interrupt not enabled. Enabling.");
            rtc.enablePORIE();
        }

        // Setup external interrupt on rising edge
        rtc.setExternalEventFiltering(EHL_FILTER_4);
        rtc.setExternalInterruptEdge(EHL_RISING);

        if (rtc.readAlarmInterruptFlag())
        {
            Serial.println("Woke up on alarm interrupt.");
        }

        if (rtc.readExternalInterruptFlag())
        {
            Serial.println("Woke up on external interrupt.");
        }

        if (rtc.readPowerOnInterruptFlag())
        {
            Serial.println("Woke up on power on interrupt.");
        }

        Serial.println("Try to sleep for one minute and wake up again.");
        RTCSleep(0, 1);
    }
}

void RTCSleep(int hours, int minutes)
{
    // Set time and alarm for next wakeup.
    // Set RTC time to start of day, because we just want a periodic interrupt and no real time.
    rtc.setTime(0, 0, 0, 1, 1, 1, 2020);

    /*********************************
      Set the alarm mode in the following way: (Parameter 5)
      0: When minutes, hours and weekday/date match (once per weekday/date)
      1: When hours and weekday/date match (once per weekday/date)
      2: When minutes and weekday/date match (once per hour per weekday/date)
      3: When weekday/date match (once per weekday/date)
      4: When hours and minutes match (once per day)
      5: When hours match (once per day)
      6: When minutes match (once per hour)
      7: All disabled – Default value
      If you want to set a weekday alarm (alm_isweekday = true), set 'alm_date_or_weekday' from 0 (Sunday) to 6 (Saturday)
    ********************************/

    // Wake up in a minute
    rtc.enableAlarmInterrupt(minutes, hours, 0, false, 4, false); // Wake up at the defined time.
    Serial.println("Go to sleep");
    delay(200); // Give it some time to send outstanding messages like serial or wifi.
    // clear Interupt flags to sleep again.
    rtc.clearAlarmInterruptFlag();
    rtc.clearPowerOnInterruptFlag();
    rtc.clearExternalInterruptFlag();
}