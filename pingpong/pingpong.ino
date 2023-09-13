#include "LedControl.h"
#include "game.h"
#include <Arduino.h>

#define DATA_PIN 2
#define CLOCK_PIN 8
#define CHIP_SELECT_PIN 4

LedControl lcd(DATA_PIN, CLOCK_PIN, CHIP_SELECT_PIN, 1);
Game game(&lcd);

void setup() {
    lcd.shutdown(0, false);
    lcd.setIntensity(0, 15);
    lcd.clearDisplay(0);

}

void loop() {

    Base* base = game.make_base(5);
    if (base == nullptr) {
        return;
    }


    game.set_base(base, 0);
    delete[] base;
    game.render_scene();
    delay(150);
    lcd.clearDisplay(0);

    /*
    // shoot
    for (int i = 0; i < 8; i++) {
        game.set_base(base, 0);
        delete[] base;

        Projectile projectile;
        projectile.x = i;
        projectile.y = i;
        

        game.set_projectile(&projectile, 0);
        game.render_scene();
        delay(150);
        lcd.clearDisplay(0);
    }

    // reflect
    for (int i = 8-1; i >= 0; i--) {
        game.set_base(base, 0);
        delete[] base;

        Projectile projectile;
        projectile.x = i;
        projectile.y = i;

        game.set_projectile(&projectile, 0);
        game.render_scene();
        delay(150);
        lcd.clearDisplay(0);
    }*/
}
