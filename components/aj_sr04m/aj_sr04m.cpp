// Credits go @at0m to on https://community.home-assistant.io/t/esphome-ultrasonic-sensor-sr04t-via-uart/458248/27 as this code is based of of his work.

// Datasheet https://wiki.dfrobot.com/A01NYUB%20Waterproof%20Ultrasonic%20Sensor%20SKU:%20SEN0313

#include "aj_sr04m.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace aj_sr04m {

static const char* const TAG = "aj_sr04m.sensor";

void AJSR04MComponent::setup() {
  read_trigger_ = 0x01;
  ESP_LOGCONFIG("", "Setup of AJSR04MComponent Finished.");
}

void AJSR04MComponent::update() {
  //this->write_byte(0x00); // Try this
  //this->write_byte(0x55); // Try this
  //this->write_byte(0x01); // Try this
  //ESP_LOGV(TAG,"Sent read request to sensor");

  this->write_byte(read_trigger_);
  ESP_LOGV("", "Sent read request to sensor using %02x", read_trigger_);
}

void AJSR04MComponent::loop() {
  while (this->available() > 0) {
    uint8_t data;
    this->read_byte(&data);

    ESP_LOGV("", "Read byte from sensor: %x", data);

    if (this->buffer_.empty() && data != 0xFF)
      continue;

    this->buffer_.push_back(data);
    if (this->buffer_.size() == 4)
      this->check_buffer_();
  }
}

void AJSR04MComponent::dump_config() {
  LOG_SENSOR("", "AJ_SR04M Sensor", this);
  LOG_UPDATE_INTERVAL(this);
}

void AJSR04MComponent::check_buffer_() {
  // AJ_SR04M_Sensor format:
  // Trigger: 0x01 (some use 0x00 or 0x55)
  // Response: Byte0          Byte1  Byte2  Byte3               Byte4
  //           Start Byte=FF  MSB    LSB    Checksum (LSB+MSB)  00 (not always?)
  //The Byte4 description might be a lie 2024.06.09 ejb

  uint8_t checksum = this->buffer_[0] + this->buffer_[1] + this->buffer_[2];
  if (this->buffer_[3] == checksum) {
    uint16_t distance = encode_uint16(this->buffer_[1], this->buffer_[2]);
    if (distance > 200) {
      float scaled_distance = distance / 10.0f;
      ESP_LOGV("", "Distance from sensor: %umm, %.1fcm", distance,
               scaled_distance);
      this->publish_state(scaled_distance);
    } else {
      ESP_LOGW("", "Invalid data read from sensor: %s",
               format_hex_pretty(this->buffer_).c_str());
    }
  } else {
    // ESP_LOGW("", "Checksum failed: %02x != %02x", checksum, this->buffer_[3]);
  }
  this->buffer_.clear();
}

}  // namespace aj_sr04m
}  // namespace esphome
