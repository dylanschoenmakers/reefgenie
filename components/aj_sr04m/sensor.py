import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    STATE_CLASS_MEASUREMENT,
    UNIT_METER,
    ICON_ARROW_EXPAND_VERTICAL,
    DEVICE_CLASS_DISTANCE,
)
import esphome.codegen as cg
from esphome.components import uart, sensor


DEPENDENCIES = ["uart"]

aj_sr04m = cg.esphome_ns.namespace("aj_sr04m")
AJSR04MComponent = aj_sr04m.class_(
    "AJSR04MComponent", cg.PollingComponent, sensor.Sensor, cg.Component, uart.UARTDevice)


CONFIG_SCHEMA = sensor.sensor_schema(
    AJSR04MComponent,
    unit_of_measurement=UNIT_METER,
    icon=ICON_ARROW_EXPAND_VERTICAL,
    accuracy_decimals=3,
    state_class=STATE_CLASS_MEASUREMENT,
    device_class=DEVICE_CLASS_DISTANCE,
).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
