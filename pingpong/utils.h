#ifndef UTILS_H
#define UTILS_H

class Coordinates {
private:
    int x, y;

public:
    Coordinates() {
        this->x = 0;
        this->y = 0;
    }
    
    ~Coordinates() {}

    void set_x(int x) {
        this->x = x;
    }

    void set_y(int y) {
        this->y = y;
    }

    int& get_x() {
        return this->x;
    }

    int& get_y() {
        return this->y;
    }

};

#endif