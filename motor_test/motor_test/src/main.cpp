#include <Arduino.h>
#include <DShotRMT.h>

DShotRMT motor(GPIO_NUM_27, DSHOT300);

// Send frames continuously for `ms` milliseconds
void hold(float percent, unsigned long ms) {
    unsigned long start = millis();
    while (millis() - start < ms) {
        if (percent <= 0) motor.sendThrottle(0);       // true 0 = stop/arm
        else              motor.sendThrottlePercent(percent);
        delay(1);
    }
}

void setup() {
    motor.begin();
    hold(0, 3000);   // arm: 3 s of zero throttle
}

void loop() {
    hold(10, 1000);  // 10% for 1 s, start low with props OFF
    hold(0, 1000);   // stop for 1 s
}