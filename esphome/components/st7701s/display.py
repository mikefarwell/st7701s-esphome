import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import display
from esphome.const import (
    CONF_ID,
    CONF_LAMBDA,
    CONF_MODEL,
    CONF_PAGES,
    CONF_INVERT_COLORS,
)
from . import ST7701S_ns

CODEOWNERS = ["@mikefarwell"]

CONF_RGB_PCLK = "RGB_PCLK"
CONF_RGB_DE = "RGB_DE"
CONF_RGB_VS = "RGB_VS"
CONF_RGB_HS = "RGB_HS"
CONF_RGB_D0 = "RGB_D0"
CONF_RGB_D1 = "RGB_D1"
CONF_RGB_D2 = "RGB_D2"
CONF_RGB_D3 = "RGB_D3"
CONF_RGB_D4 = "RGB_D4"
CONF_RGB_D5 = "RGB_D5"
CONF_RGB_D6 = "RGB_D6"
CONF_RGB_D7 = "RGB_D7"
CONF_RGB_D8 = "RGB_D8"
CONF_RGB_D9 = "RGB_D9"
CONF_RGB_D10 = "RGB_D10"
CONF_RGB_D11 = "RGB_D11"
CONF_RGB_D12 = "RGB_D12"
CONF_RGB_D13 = "RGB_D13"
CONF_RGB_D14 = "RGB_D14"
CONF_RGB_D15 = "RGB_D15"
CONF_LCD_BL = "LCD_BL"
CONF_DEVICE_WIDTH = "device_width"
CONF_DEVICE_HEIGHT = "device_height"
CONF_ROW_START = "row_start"
CONF_COL_START = "col_start"
CONF_EIGHT_BIT_COLOR = "eight_bit_color"
CONF_USE_BGR = "use_bgr"

PARALLELST7701S = ST7701S_ns.class_(
    "ST7701S", cg.PollingComponent, display.DisplayBuffer
)
ST7701SModel = ST7701S_ns.enum("ST7701SModel")

MODELS = {
    "DEFAULT": ST7701SModel.ST7701S_DEFAULT
}
ST7701S_MODEL = cv.enum(MODELS, upper=True, space="_")


ST7701S_SCHEMA = display.FULL_DISPLAY_SCHEMA.extend(
    {
        cv.Required(CONF_MODEL): ST7701S_MODEL,
    }
).extend(cv.polling_component_schema("1s"))

CONFIG_SCHEMA = cv.All(
    ST7701S_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(PARALLELST7701S),
            cv.Required(CONF_RGB_PCLK): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_DE): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_VS): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_HS): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D0): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D1): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D2): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D3): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D4): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D5): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D6): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D7): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D8): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D9): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D10): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D11): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D12): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D13): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D14): pins.gpio_output_pin_schema,
            cv.Required(CONF_RGB_D15): pins.gpio_output_pin_schema,
            cv.Required(CONF_LCD_BL): pins.gpio_output_pin_schema,
            cv.Required(CONF_DEVICE_WIDTH): cv.int_,
            cv.Required(CONF_DEVICE_HEIGHT): cv.int_,
            cv.Required(CONF_COL_START): cv.int_,
            cv.Required(CONF_ROW_START): cv.int_,
            cv.Optional(CONF_EIGHT_BIT_COLOR, default=False): cv.boolean,
            cv.Optional(CONF_USE_BGR, default=False): cv.boolean,
            cv.Optional(CONF_INVERT_COLORS, default=False): cv.boolean,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    cv.has_at_most_one_key(CONF_PAGES, CONF_LAMBDA),
)


async def setup_ST7701S(var, config):
    await display.register_display(var, config)

    if CONF_RESET_PIN in config:
        reset = await cg.gpio_pin_expression(config[CONF_RESET_PIN])
        cg.add(var.set_reset_pin(reset))
    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(display.DisplayRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))


async def to_code(config):
    var = cg.new_Pvariable(
        config[CONF_ID],
        config[CONF_MODEL],
        config[CONF_DEVICE_WIDTH],
        config[CONF_DEVICE_HEIGHT],
        config[CONF_COL_START],
        config[CONF_ROW_START],
        config[CONF_EIGHT_BIT_COLOR],
        config[CONF_USE_BGR],
        config[CONF_INVERT_COLORS],
    )
    await setup_ST7701S(var, config)

    dc = await cg.gpio_pin_expression(config[CONF_DC_PIN])
    cg.add(var.set_dc_pin(dc))
