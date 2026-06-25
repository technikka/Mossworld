#ifndef POSITION_H
#define POSITION_H

struct Position {
    int x;
    int y;

    // defining equality of two positions
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

#endif