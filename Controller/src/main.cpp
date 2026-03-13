#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#include "../../Communication/communication.hpp"

rand_pack rand_data;
movement_pack movement_data;

uint8_t receiver_addr[] = {0x28, 0x05, 0xA5, 0x35, 0x7B, 0x58}; // example MAC

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failure");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(onDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiver_addr, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  rand_data.x++;
  rand_data.y++;

  esp_err_t result = esp_now_send(receiver_addr, (uint8_t *)&rand_data, sizeof(rand_data));

  if (result != ESP_OK) {
    Serial.println("Send error");
  }

  delay(1000);
}
