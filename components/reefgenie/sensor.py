import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    ENTITY_CATEGORY_DIAGNOSTIC,
    ENTITY_CATEGORY_NONE,
    DEVICE_CLASS_EMPTY,
    STATE_CLASS_MEASUREMENT,
    CONF_ID,
)

from . import ReefGenie


DEPENDENCIES = ["reefgenie"]

CONF_CALCIUM_TESTED = "calcium_tested"
CONF_ALKALINITY_TESTED = "alkalinity_tested"
CONF_MAGNESIUM_TESTED = "magnesium_tested"
CONF_CURRENT_DROPS_COUNTED = "current_drops_counted"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(ReefGenie),
        cv.Optional(CONF_CALCIUM_TESTED): sensor.sensor_schema(
            unit_of_measurement="ppm",
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
            state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_NONE,
        ),
        cv.Optional(CONF_ALKALINITY_TESTED): sensor.sensor_schema(
            unit_of_measurement="dKH",
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_EMPTY,
            state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_NONE,
        ),
        cv.Optional(CONF_CURRENT_DROPS_COUNTED): sensor.sensor_schema(
            unit_of_measurement="drops",
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
            state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ID])

    if CONF_CALCIUM_TESTED in config:
        sens = await sensor.new_sensor(config[CONF_CALCIUM_TESTED])
        cg.add(parent.set_calcium_tested(sens))

    if CONF_ALKALINITY_TESTED in config:
        sens = await sensor.new_sensor(config[CONF_ALKALINITY_TESTED])
        cg.add(parent.set_alkalinity_tested(sens))

    if CONF_MAGNESIUM_TESTED in config:
        sens = await sensor.new_sensor(config[CONF_MAGNESIUM_TESTED])
        cg.add(parent.set_magnesium_tested(sens))

    if CONF_CURRENT_DROPS_COUNTED in config:
        sens = await sensor.new_sensor(config[CONF_CURRENT_DROPS_COUNTED])
        cg.add(parent.set_current_drops_counted(sens))
