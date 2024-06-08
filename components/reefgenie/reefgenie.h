#pragma once

#include <utility>
#include "esphome/components/i2c/i2c.h"

#include "esphome/components/a4988/a4988.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace reefgenie {

static const uint16_t TANK_POS = 123;
static const uint16_t REAGENT_A_POS = 246;
static const uint16_t REAGENT_B_POS = 369;
static const uint16_t REAGENT_C_POS = 492;
static const uint16_t DRAIN_POS = 615;

enum State {
  IDLE,
  CALCIUM_TEST,
  ALKALINITY_TEST,
  MAGNESIUM_TEST,
};

enum TritationCommand {
  SELECT_VALVE,
  PUMP_IN,
  PUMP_OUT,
  STIR,
  WAIT,
  DOSE_DROPS,
  TEST_COLOR,
  CALCULATE_CONCENTRATION
};

union TritationCommandParameter {
  uint16_t valve;
  float amount;
  unsigned long duration;
};

struct TritationStep {
  TritationCommand command;
  TritationCommandParameter parameter;
  const char* description;
};

class ReefGenie : public PollingComponent, public i2c::I2CDevice {
 public:
  float get_setup_priority() const override;

  void setup() override;
  void loop() override;
  void update() override;

  // Stepper motor control
  void set_valve(stepper::Stepper* stepper) { this->valve_ = stepper; }
  void set_pump(stepper::Stepper* stepper) { this->pump_ = stepper; }
  void set_sleep_pin(GPIOPin* sleep_pin) { this->sleep_pin_ = sleep_pin; }

  // User commands
  void start_test(State state);
  void reset();
  void next_step();

  TritationStep current_step();
  std::vector<TritationStep> current_tritation();

  void select_valve(uint16_t valve);
  void pump(float amount);
  void stir();
  void wait(unsigned long duration);
  void calculate_concentration();

  int32_t valve_current_position{0};
  int32_t valve_target_position{0};

  void set_calcium_tested(sensor::Sensor* calcium_tested) {
    calcium_tested_ = calcium_tested;
  }
  void set_alkalinity_tested(sensor::Sensor* alkalinity_tested) {
    alkalinity_tested_ = alkalinity_tested;
  }
  void set_magnesium_tested(sensor::Sensor* magnesium_tested) {
    magnesium_tested_ = magnesium_tested;
  }
  void set_current_drops_counted(sensor::Sensor* current_drops_counted) {
    current_drops_counted_ = current_drops_counted;
  }

  void set_current_status(text_sensor::TextSensor* current_status) {
    current_status_ = current_status;
  }
  void set_current_operation(text_sensor::TextSensor* current_operation) {
    current_operation_ = current_operation;
  }

 protected:
  stepper::Stepper* valve_{nullptr};
  stepper::Stepper* pump_{nullptr};
  GPIOPin* sleep_pin_{nullptr};
  bool sleep_pin_state_;

  State state_{IDLE};
  uint8_t current_step_{0};
  uint16_t drops_counted_{0};
  uint32_t last_action_time_{0};
  uint8_t step_execution_{0};

  sensor::Sensor* calcium_tested_{nullptr};
  sensor::Sensor* alkalinity_tested_{nullptr};
  sensor::Sensor* current_drops_counted_{nullptr};
  sensor::Sensor* magnesium_tested_{nullptr};

  text_sensor::TextSensor* current_status_{nullptr};
  text_sensor::TextSensor* current_operation_{nullptr};

  std::vector<TritationStep> AlkalinityTritation = {
      {SELECT_VALVE, {.valve = TANK_POS}, "Select tank valve"},
      {PUMP_IN, {.amount = 2.0}, "Pumping in 2ml tank water"},
      {WAIT, {.duration = 2000}, "Waiting for 2 seconds"},
      {SELECT_VALVE, {.valve = REAGENT_A_POS}, "Select reagent A valve"},
      {PUMP_IN, {.amount = 2.0}, "Pumping in 2ml reagent A"},
      {CALCULATE_CONCENTRATION, {}, "Calculating calcium concentration"},
      {WAIT,
       {.duration = 2000},
       "Finished, waiting for 2 seconds before draining"},
      {SELECT_VALVE, {.valve = DRAIN_POS}, "Select drain valve"},
      {PUMP_OUT, {.amount = 2.0}, "Pumping out 2ml of water"},
  };

  std::vector<TritationStep> CalciumTritation = {
      {SELECT_VALVE, {.valve = TANK_POS}, "Select tank valve"},
      {PUMP_IN, {.amount = 2.0}, "Pumping in 2ml tank water"},
      {WAIT, {.duration = 2000}, "Waiting for 2 seconds"},
      {SELECT_VALVE, {.valve = REAGENT_B_POS}, "Select reagent A valve"},
      {PUMP_IN, {.amount = 2.0}, "Pumping in 2ml reagent A"},
      {CALCULATE_CONCENTRATION, {}, "Calculating calcium concentration"},
      {WAIT,
       {.duration = 2000},
       "Finished, waiting for 2 seconds before draining"},
      {SELECT_VALVE, {.valve = DRAIN_POS}, "Select drain valve"},
      {PUMP_OUT, {.amount = 2.0}, "Pumping out 2ml of water"},
  };

  std::vector<TritationStep> MagnesiumTritation = {
      {SELECT_VALVE, {TANK_POS}},
      {PUMP_IN, {.amount = 2.0}},
  };
};
template <typename... Ts>
class TestCalciumAction : public Action<Ts...> {
 public:
  TestCalciumAction(ReefGenie* reefgenie) : reefgenie_(reefgenie) {}

  void play(Ts... x) override { this->reefgenie_->start_test(CALCIUM_TEST); }

 protected:
  ReefGenie* reefgenie_;
};
template <typename... Ts>
class TestAlkalinityCalciumAction : public Action<Ts...> {
 public:
  TestAlkalinityCalciumAction(ReefGenie* reefgenie) : reefgenie_(reefgenie) {}

  void play(Ts... x) override { this->reefgenie_->start_test(ALKALINITY_TEST); }

 protected:
  ReefGenie* reefgenie_;
};
template <typename... Ts>
class TestMagnesiumCalciumAction : public Action<Ts...> {
 public:
  TestMagnesiumCalciumAction(ReefGenie* reefgenie) : reefgenie_(reefgenie) {}

  void play(Ts... x) override { this->reefgenie_->start_test(MAGNESIUM_TEST); }

 protected:
  ReefGenie* reefgenie_;
};

}  // namespace reefgenie
}  // namespace esphome