#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#include "../../Communication/communication.hpp"

rand_pack rand_data;
movement_pack movement_data;

unsigned long prev_time = 0;

void onRandDataReceive(const uint8_t *mac, const uint8_t *incomingData, int len) {
    packet_type type = *(packet_type*)incomingData;

    switch (type) {
        case RAND_PACK:
            if (len == sizeof(rand_pack)) {
                memcpy(&rand_data, incomingData, sizeof(rand_data));
            }
            break;

        case MOVEMENT_PACK:
            if (len == sizeof(movement_pack)) {
                memcpy(&movement_data, incomingData, sizeof(movement_data));
            }
            break;

        default:
            Serial.println("Unknown packet type");
            break;
    }
}


void setup() {
  Serial.begin(115200);
  delay(200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error: Failed init esp_now");
    return;
  }

  esp_now_register_recv_cb(onRandDataReceive);
}

void loop() {
  unsigned long now = millis();
  if (now - prev_time >= 1000) {
    prev_time = now;

    Serial.print("x = ");
    Serial.println(rand_data.x);

    Serial.print("y = ");
    Serial.println(rand_data.y);
  }
}
