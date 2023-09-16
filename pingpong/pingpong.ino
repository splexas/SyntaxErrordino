#include "LedControl.h"
#include "game.h"
#include <Arduino.h>

#define DATA_PIN 2
#define CLOCK_PIN 8
#define CHIP_SELECT_PIN 4

LedControl lcd(DATA_PIN, CLOCK_PIN, CHIP_SELECT_PIN, 2);
Game game(&lcd);

void setup() {
    for (int i = 0; i < 2; i++) {
        lcd.shutdown(i, false);
        lcd.setIntensity(i, 15);
        lcd.clearDisplay(i);
    }
}

void loop() {
    Coordinates p1_platform = game.make_platform(0, 1);
    game.set_platform(&p1_platform, 0);

    Coordinates p2_platform = game.make_platform(1, 2);
    game.set_platform(&p2_platform, 1);

    // shooting the projectile and bouncing it...

    game.render_scene();
}
