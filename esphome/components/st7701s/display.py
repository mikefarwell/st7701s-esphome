import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import core, pins
from esphome.components import display, font
from esphome.components.display import validate_rotation
from esphome.core import CORE, HexInt
from esphome.const import (
    CONF_ID,
    CONF_LAMBDA,
    CONF_MODEL,
    CONF_RAW_DATA_ID,
    CONF_PAGES,
    CONF_RESET_PIN,
    CONF_DIMENSIONS,
    CONF_WIDTH,
    CONF_HEIGHT,
    CONF_ROTATION,
    #CONF_MIRROR_X,
    #CONF_MIRROR_Y,
    #CONF_SWAP_XY,
    #CONF_COLOR_ORDER,
    #CONF_OFFSET_HEIGHT,
    #CONF_OFFSET_WIDTH,
    #CONF_TRANSFORM,
    #CONF_INVERT_COLORS,
)

def AUTO_LOAD():
    if CORE.is_esp32:
        return ["psram"]
    return []

CODEOWNERS = ["@mikefarwell"]

st7701s_ns = cg.esphome_ns.namespace("st7701s")
ST7701SDisplay = st7701s_ns.class_(
    "ST7701SDisplay",
    cg.PollingComponent,
    display.Display,
    display.DisplayBuffer,
)

ST7701SColorMode = st7701s_ns.enum("ST7701SColorMode")
ColorOrder = display.display_ns.enum("ColorMode")

MODELS = {
    "DEFAULT": st7701s_ns.class_("DEFAULT", ST7701SDisplay)
}

COLOR_ORDERS = {
    "RGB": ColorOrder.COLOR_ORDER_RGB,
    "BGR": ColorOrder.COLOR_ORDER_BGR,
}

COLOR_PALETTE = cv.one_of("NONE", "GRAYSCALE", "IMAGE_ADAPTIVE")

CONF_LED_PIN = "led_pin"
CONF_COLOR_PALETTE_IMAGES = "color_palette_images"
CONF_INVERT_DISPLAY = "invert_display"


def _validate(config):
    if CORE.is_esp8266 and config.get(CONF_MODEL) not in [
        "M5STACK",
        "TFT_2.4",
        "TFT_2.4R",
        "ILI9341",
        "ILI9342",
        "ST7789V",
    ]:
        raise cv.Invalid(
            "Provided model can't run on ESP8266. Use an ESP32 with PSRAM onboard"
        )
    return config


CONFIG_SCHEMA = cv.All(
    font.validate_pillow_installed,
    display.FULL_DISPLAY_SCHEMA
        .extend(
            {
                cv.GenerateID(): cv.declare_id(ST7701SDisplay),
                cv.Required(CONF_MODEL): cv.enum(MODELS, upper=True, space="_"),
                cv.Optional(CONF_DIMENSIONS): cv.Any(
                    cv.dimensions,
                    cv.Schema(
                        {
                            cv.Required(CONF_WIDTH): cv.int_,
                            cv.Required(CONF_HEIGHT): cv.int_,
                            #cv.Optional(CONF_OFFSET_HEIGHT, default=0): cv.int_,
                            #cv.Optional(CONF_OFFSET_WIDTH, default=0): cv.int_,
                        }
                    ),
                ),
                cv.Exclusive(CONF_ROTATION, CONF_ROTATION): validate_rotation,
                #cv.Exclusive(CONF_TRANSFORM, CONF_ROTATION): cv.Schema(
                    #{
                        #cv.Optional(CONF_SWAP_XY, default=False): cv.boolean,
                        #cv.Optional(CONF_MIRROR_X, default=False): cv.boolean,
                        #cv.Optional(CONF_MIRROR_Y, default=False): cv.boolean,
                    #}
                #),
            }
        )
    .extend(cv.polling_component_schema("1s")),
    cv.has_at_most_one_key(CONF_PAGES, CONF_LAMBDA),
    _validate,
)


async def to_code(config):
    rhs = MODELS[config[CONF_MODEL]].new()
    var = cg.Pvariable(config[CONF_ID], rhs)

    await display.register_display(var, config)
    #if CONF_COLOR_ORDER in config:
    #    cg.add(var.set_color_order(COLOR_ORDERS[config[CONF_COLOR_ORDER]]))
    #if CONF_TRANSFORM in config:
    #    transform = config[CONF_TRANSFORM]
        #cg.add(var.set_swap_xy(transform[CONF_SWAP_XY]))
        #cg.add(var.set_mirror_x(transform[CONF_MIRROR_X]))
        #cg.add(var.set_mirror_y(transform[CONF_MIRROR_Y]))

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(display.DisplayRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))

    if CONF_RESET_PIN in config:
        reset = await cg.gpio_pin_expression(config[CONF_RESET_PIN])
        cg.add(var.set_reset_pin(reset))

    if CONF_DIMENSIONS in config:
        dimensions = config[CONF_DIMENSIONS]
        if isinstance(dimensions, dict):
            cg.add(var.set_dimensions(dimensions[CONF_WIDTH], dimensions[CONF_HEIGHT]))
            #cg.add(
            #    var.set_offsets(
            #        dimensions[CONF_OFFSET_WIDTH], dimensions[CONF_OFFSET_HEIGHT]
            #    )
            #)
        else:
            (width, height) = dimensions
            cg.add(var.set_dimensions(width, height))

    rhs = None
        cg.add(var.set_buffer_color_mode(ST7701SColorMode.BITS_16))

    if rhs is not None:
        prog_arr = cg.progmem_array(config[CONF_RAW_DATA_ID], rhs)
        cg.add(var.set_palette(prog_arr))
