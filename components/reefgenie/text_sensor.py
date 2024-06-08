import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import (
    CONF_ID,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

from . import ReefGenie

DEPENDENCIES = ["reefgenie"]

CONF_CURRENT_STATUS = "current_status"
CONF_CURRENT_OPERATION = "current_operation"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(ReefGenie),
        cv.Optional(CONF_CURRENT_STATUS): text_sensor.text_sensor_schema(
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_CURRENT_OPERATION): text_sensor.text_sensor_schema(
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ID])

    if CONF_CURRENT_STATUS in config:
        sens = await text_sensor.new_text_sensor(config[CONF_CURRENT_STATUS])
        cg.add(parent.set_current_status(sens))

    if CONF_CURRENT_OPERATION in config:
        sens = await text_sensor.new_text_sensor(config[CONF_CURRENT_OPERATION])
        cg.add(parent.set_current_operation(sens))
