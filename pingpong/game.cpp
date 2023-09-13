#include "game.h"
#include <string.h>

Game::Game(LedControl* lcd) {
    this->lcd = lcd;

    this->player_projectile = {0,0};

    this->player_bases = new Base[2];
    for (int i = 0; i < 2; i++) {
        for (int a = 0; a < 2; a++) {
            this->player_bases[i].platform[a][0] = 0;
            this->player_bases[i].platform[a][1] = 0;
        }

    }
    
}

Game::~Game() {
    delete[] this->player_bases;
}

Projectile* Game::get_projectile() {
    return &this->player_projectile;
}

Base* Game::get_base(unsigned int player_index) {
    return &this->player_bases[player_index];
}

void Game::set_projectile(Projectile* projectile) {
    this->player_projectile = *projectile;
}

void Game::set_base(Base* base, unsigned int player_index) {
    memcpy(&this->player_bases[player_index], base, sizeof(Base));
}

Projectile* Game::make_projectile(Direction direction, Trajectory trajectory) {
    if (direction == Direction::LEFT && this->player_projectile.x == 0) {
        return nullptr;
    }

    // more here
}

Base* Game::make_base(unsigned int starting_point) {
    // makes the base on the side

    if (starting_point % 2 == 0) {
        return nullptr;
    }

    Base* base = new Base;
    
    base->platform[0][0] = 0;
    base->platform[0][1] = starting_point - 1;

    base->platform[1][0] = 0;
    base->platform[1][1] = starting_point;

    base->platform[2][0] = 0;
    base->platform[2][1] = starting_point + 1;

    return base;
}

bool Game::check_collision() {
    for (int i = 0; i < 2; i++) {
        for (int a = 0; a < 3; a++) {
            if (this->player_bases[i].platform[a][0] == this->player_projectile.x || this->player_bases[i].platform[a][1] == this->player_projectile.y) {
                return true;
            }
        }
    }

    return false;
}

void Game::render_scene() {
    this->lcd->setLed(0, this->player_projectile.x, this->player_projectile.y, true);

    for (int i = 0; i < 3; i++) {
        this->lcd->setLed(0, this->player_bases[0].platform[i][0], this->player_bases[0].platform[i][1], true);
    }

    /*for (int i = 0; i < 2; i++) {
        this->lcd->setLed(0, this->player_projectiles[i].x, this->player_projectiles[i].y, true);
    }*/
}