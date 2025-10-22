#include "Umfeld.h"
#include "gpio-linux.h"

using namespace umfeld;

bool is_led_on    = false;
const int led_id  = GPIO_2;

void settings() {
    size(1024, 768);
}

void setup() {
    noStroke();
    pinMode(led_id, OUTPUT);
}

void draw() {
    background(0.85f);
    const float size = 100.0;
    const float x    = width / 2.0;
    const float y    = height / 2.0;

    if (is_led_on) {
        fill(1, 0.25, 0.35);
    } else {
        fill(0.2, 0.2, 0.2);
    }
    circle(x, y, size);

    if (frameCount % 120 == 0) {
        digitalWrite(led_id, true);
        is_led_on = true;
    }
    if (frameCount % 120 == 60) {
        digitalWrite(led_id, false);
        is_led_on = false;
    }
}
