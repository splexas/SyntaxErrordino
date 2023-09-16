#ifndef GAME_H
#define GAME_H

#include "LedControl.h"
#include "utils.h"

enum class Direction {
    LEFT,
    RIGHT
};

enum class Trajectory {
    UP,
    DOWN,
    CENTER
};

class Game {
private:
    LedControl* lcd;

    Coordinates player_projectile; // will be used for both players
    Coordinates player1_platform; // stores just the middle
    Coordinates player2_platform; // stores just the middle

public:
    Game(LedControl* lcd);
    ~Game();

    Coordinates* get_projectile();
    Coordinates* get_platform(unsigned int player_index);

    void set_projectile(Coordinates* projectile);
    void set_platform(Coordinates* platform, unsigned int player_index);

    Coordinates make_projectile(Direction direction, Trajectory trajectory); // makes a new dynamic projectile object with next path coordinates
    Coordinates make_platform(unsigned int player_index, unsigned int starting_point); // makes a platform (it can make coordinates with negative values) on where the player is supposed to hit the projectile
    
    int check_collision(); // returns -1 if the projectile hasnt collided with anything, returns the player index otherwise.

    void set_led(int x, int y); // generic for 2 matrix devices (8x16)
    void render_scene(); // renders the scene (the projectile and the players' platforms)
};

#endif