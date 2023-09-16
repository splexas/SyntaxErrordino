#include "game.h"
#include <string.h>

Game::Game(LedControl* lcd) {
    this->lcd = lcd;
}

Game::~Game() {}

Coordinates* Game::get_projectile() {
    return &this->player_projectile;
}

Coordinates* Game::get_platform(unsigned int player_index) {
    if (player_index == 0) {
        return &this->player1_platform;
    }

    return &this->player2_platform;
}

void Game::set_projectile(Coordinates* projectile) {
    this->player_projectile = *projectile;
    return;
}

void Game::set_platform(Coordinates* platform, unsigned int player_index) {
    if (player_index == 0) {
        this->player1_platform = *platform;
        return;
    }

    this->player2_platform = *platform;
    return;
}

Coordinates Game::make_projectile(Direction direction, Trajectory trajectory) {
    Coordinates projectile;

    if (direction == Direction::LEFT) {
        projectile.set_x(this->player_projectile.get_x() - 1);
    } else if (direction == Direction::RIGHT) {
        projectile.set_x(this->player_projectile.get_x() + 1);
    }

    // if its center, dont do anything to y 
    if (trajectory == Trajectory::DOWN) {
        projectile.set_y(this->player_projectile.get_y() - 1);
    } else if (trajectory == Trajectory::UP) {
        projectile.set_y(this->player_projectile.get_y() + 1);
    }

    return projectile;
}

Coordinates Game::make_platform(unsigned int player_index, unsigned int starting_point) {
    Coordinates platform;

    int y = 0;
    if (player_index == 1) {
        y = 15; 
    }

    platform.set_x(starting_point);
    platform.set_y(y);

    return platform;
}

int Game::check_collision() {
    // the projectile isnt in line with any of the players' platforms
    if (this->player1_platform.get_y() < this->player_projectile.get_y() && this->player2_platform.get_y() > this->player_projectile.get_y()) {
        return -1;
    }

    if (this->player1_platform.get_x() - 1 <= this->player_projectile.get_x() && this->player1_platform.get_x() + 1 >= this->player_projectile.get_x()) {
        return 0;
    }

    if (this->player2_platform.get_x() - 1 <= this->player_projectile.get_x() && this->player2_platform.get_x() + 1 >= this->player_projectile.get_x()) {
        return 1;
    }


    return -1;
}

void Game::set_led(int x, int y) {
    int device_index = 0;

    if (y > 7) {
        device_index = 1;
        y -= 8;
    }

    this->lcd->setLed(device_index, y, x, true);
    return;
}

void Game::render_scene() {
    // projectile
    this->set_led(this->player_projectile.get_x(), this->player_projectile.get_y());

    // player1 platform
    this->set_led(this->player1_platform.get_x() - 1, this->player1_platform.get_y());
    this->set_led(this->player1_platform.get_x(), this->player1_platform.get_y());
    this->set_led(this->player1_platform.get_x() + 1, this->player1_platform.get_y());

    // player2 platform
    this->set_led(this->player2_platform.get_x() - 1, this->player2_platform.get_y());
    this->set_led(this->player2_platform.get_x(), this->player2_platform.get_y());
    this->set_led(this->player2_platform.get_x() + 1, this->player2_platform.get_y());

    return;
}