// g++ main.cpp $(pkg-config --cflags --libs libgpiod) -o gpio_test

// main.cpp  (libgpiod v2)
#include <gpiod.h>
#include <unistd.h>

int main() {
    const char *chip_path = "/dev/gpiochip0";
    const unsigned off = 2; // BCM 2

    gpiod_chip *chip = gpiod_chip_open(chip_path);
    if (!chip) return 1;

    gpiod_line_settings *settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

    gpiod_line_config *lcfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(lcfg, &off, 1, settings);

    gpiod_request_config *rcfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(rcfg, "blink");

    gpiod_line_request *req = gpiod_chip_request_lines(chip, rcfg, lcfg);
    if (!req) return 2;

    for (int i = 0; i < 10; ++i) {
        gpiod_line_request_set_value(req, off, GPIOD_LINE_VALUE_ACTIVE);
        usleep(500000);
        gpiod_line_request_set_value(req, off, GPIOD_LINE_VALUE_INACTIVE);
        usleep(500000);
    }

    gpiod_line_request_release(req);
    gpiod_request_config_free(rcfg);
    gpiod_line_config_free(lcfg);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);
    return 0;
}

