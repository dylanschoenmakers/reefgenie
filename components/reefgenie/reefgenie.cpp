
#include "reefgenie.h"
#include "esphome/core/log.h"

namespace esphome {
namespace reefgenie {

static const char* const TAG = "reefgenie";

float ReefGenie::get_setup_priority() const {
  return setup_priority::IO;
}

void ReefGenie::setup() {
  ESP_LOGI(TAG, "Setting up ReefGenie...");

  ESP_LOGI(TAG, "Setting up stepper pins...");
  if (this->sleep_pin_ != nullptr) {
    this->sleep_pin_->setup();
    this->sleep_pin_->digital_write(true);
    this->sleep_pin_state_ = true;
  }
}

void ReefGenie::loop() {
  if (this->state_ == IDLE) {
    return;
  }

  // Currently running a test, get the current step for the specific test and run it
  TritationStep step = this->current_step();
  switch (step.command) {
    case SELECT_VALVE:
      this->select_valve(step.parameter.valve);
      break;
    case PUMP_IN:
      this->pump(step.parameter.amount);
      break;
    case PUMP_OUT:
      this->pump(step.parameter.amount * -1);
      break;
    case WAIT:
      this->wait(step.parameter.duration);
      break;
    case CALCULATE_CONCENTRATION:
      this->calculate_concentration();
      break;
  }
}

TritationStep ReefGenie::current_step() {
  return this->current_tritation()[this->current_step_];
}

// get the current tritation steps based on state_
std::vector<TritationStep> ReefGenie::current_tritation() {
  switch (this->state_) {
    case CALCIUM_TEST:
      return CalciumTritation;
    case ALKALINITY_TEST:
      return AlkalinityTritation;
    case MAGNESIUM_TEST:
      return MagnesiumTritation;
  }

  return CalciumTritation;
}

void ReefGenie::update() {
  // this->alkalinity_tested_->publish_state(8.1);
  // this->calcium_tested_->publish_state(500);
  // this->current_drops_counted_->publish_state(5);
}

void ReefGenie::start_test(State state) {
  ESP_LOGD(TAG, "Starting test for %d", state);
  if (this->state_ != IDLE) {
    ESP_LOGD(TAG, "Already running a test");
    return;
  }
  this->state_ = state;
  switch (state) {
    case CALCIUM_TEST:
      this->current_status_->publish_state("Testing Calcium");
      break;
    case ALKALINITY_TEST:
      this->current_status_->publish_state("Testing Alkalinity");
      break;
    case MAGNESIUM_TEST:
      this->current_status_->publish_state("Testing Magnesium");
      break;
  }
}

void ReefGenie::next_step() {
  this->step_execution_ = 0;
  // get max steps for the current test
  std::vector<TritationStep> tritation = this->current_tritation();
  if (this->current_step_ > tritation.size()) {
    // set state to IDLE
    this->state_ = IDLE;
    this->current_status_->publish_state("Test completed");
    this->current_step_ = 0;
  }

  // publish the current step to the current_operation_ text sensor
  TritationStep step = this->current_step();
  if (step.description != nullptr) {
    this->current_operation_->publish_state(step.description);
  }

  this->current_step_++;
  this->last_action_time_ = millis();
  ESP_LOGD(TAG, "Setting step %d of total %d steps", this->current_step_, tritation.size());
}

void ReefGenie::select_valve(uint16_t valve) {
  if (this->step_execution_ == 0) {
    // set the target position for the valve
    this->valve_->set_target(valve);
    this->step_execution_++;
  }
  // after 2 seconds, move to next step
  if (millis() - this->last_action_time_ > 2000) {
    this->next_step();
  }
}

void ReefGenie::pump(float amount) {
  // amount is in ml, while the pump is in steps
  // 1 ml = 200 steps
  int32_t target = amount * 200;

  if (this->step_execution_ == 0) {
    this->pump_->set_target(target);
    this->step_execution_++;
  }

  // wait until the pump has finished
  if (this->pump_->current_position == target) {
    this->next_step();
  }
}

void ReefGenie::wait(unsigned long duration) {
  if (millis() - this->last_action_time_ > duration) {
    this->next_step();
  }
}

void ReefGenie::calculate_concentration() {
  if (this->step_execution_ == 0) {
    this->calcium_tested_->publish_state(450);
    this->step_execution_++;
  }
  // calculate the concentration
  this->next_step();
}

}  // namespace reefgenie
}  // namespace esphome
