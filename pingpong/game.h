#ifndef GAME_H
#define GAME_H

#include "LedControl.h"

struct Projectile {
    int x, y;
};

struct Base {
    int platform[3][2];
};

enum class Direction {
    LEFT,
    RIGHT,
    CENTER
};

enum class Trajectory {
    UP,
    DOWN
};

class Game {
private:
    LedControl* lcd;

    Projectile player_projectile; // will be used for both players
    Base* player_bases;

public:
    Game(LedControl* lcd);
    ~Game();

    Projectile* get_projectile();
    Base* get_base(unsigned int player_index);

    void set_projectile(Projectile* projectile);
    void set_base(Base* base, unsigned int player_index);

    Projectile* make_projectile(Direction direction, Trajectory trajectory); // makes a new dynamic projectile object with next path coordinates
    Base* make_base(unsigned int starting_point); // makes a platform on where the player is supposed to hit the projectile
    
    bool check_collision(); // checks if the projectile has collided with any of the players' platforms

    void render_scene();
};

#endif