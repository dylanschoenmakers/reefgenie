import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation, pins
from esphome.components import i2c, output, binary_sensor, sensor, stepper
from esphome.automation import maybe_simple_id
from esphome.const import (CONF_ID, CONF_SLEEP_PIN)

MULTI_CONF = True

CONF_REEF_GENIE_ID = 'reef_genie_id'

AUTO_LOAD = ["fan", 'binary_sensor', 'sensor', 'text_sensor', 'stepper']

DEPENDENCIES = ['i2c']

reefgenie_ns = cg.esphome_ns.namespace('reefgenie')
ReefGenie = reefgenie_ns.class_(
    'ReefGenie', cg.PollingComponent, i2c.I2CDevice)

TestCalciumAction = reefgenie_ns.class_(
    "TestCalciumAction", automation.Action
)
TestAlkalinityAction = reefgenie_ns.class_(
    "TestAlkalinityAction", automation.Action
)
TestMagnesiumAction = reefgenie_ns.class_(
    "TestMagnesiumAction", automation.Action
)

CONF_VALVE_ID = 'valve_id'
CONF_PUMP_ID = 'pump_id'

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(ReefGenie),
            cv.Required(CONF_VALVE_ID): cv.use_id(stepper.Stepper),
            cv.Required(CONF_PUMP_ID): cv.use_id(stepper.Stepper),
            cv.Optional(CONF_SLEEP_PIN): pins.gpio_output_pin_schema,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(103))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    valve = await cg.get_variable(config[CONF_VALVE_ID])
    cg.add(var.set_valve(valve))
    pump = await cg.get_variable(config[CONF_PUMP_ID])
    cg.add(var.set_pump(pump))

    if sleep_pin_config := config.get(CONF_SLEEP_PIN):
        sleep_pin = await cg.gpio_pin_expression(sleep_pin_config)
        cg.add(var.set_sleep_pin(sleep_pin))


REEFGENIE_ACTION_SCHEMA = maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.use_id(ReefGenie),
        cv.Optional("button"): binary_sensor.binary_sensor_schema(),
    }
)


@automation.register_action("reefgenie.test_calcium", TestCalciumAction, REEFGENIE_ACTION_SCHEMA)
async def test_calcium_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)


@automation.register_action("reefgenie.test_alkalinity", TestAlkalinityAction, REEFGENIE_ACTION_SCHEMA)
async def test_alkalinity_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)


@automation.register_action("reefgenie.test_magnesium", TestMagnesiumAction, REEFGENIE_ACTION_SCHEMA)
async def test_magnesium_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)
