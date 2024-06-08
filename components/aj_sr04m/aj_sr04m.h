#pragma once

#include <vector>

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

#include "esphome.h"

namespace esphome {
namespace aj_sr04m {

class AJSR04MComponent : public sensor::Sensor,
                         public PollingComponent,
                         public uart::UARTDevice {
 public:
  AJSR04MComponent() : PollingComponent(60000) {}
  void setup() override;
  void update() override;

  void loop() override;

  void dump_config() override;

 protected:
  std::uint8_t read_trigger_;
  std::vector<uint8_t> buffer_;
  void check_buffer_();
};  //class AJ_SR04M_Sensor2

}  //namespace aj_sr04m
}  //namespace esphome