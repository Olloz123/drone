#include <Arduino.h>
#include <ESP32Servo.h>

Servo esc;

void setup()
{
    Serial.begin(115200);

    esc.setPeriodHertz(50);
    esc.attach(25, 1000, 2000);

    Serial.println("Sending minimum throttle");

    for(int i = 0; i < 500; i++)
    {
        esc.writeMicroseconds(1000);
        delay(20);
    }

    Serial.println("Spin");

    esc.writeMicroseconds(1600);
}

void loop()
{
}