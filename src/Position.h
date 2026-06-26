#ifndef POSITION_H
#define POSITION_H

struct Position {
    int x;
    int y;

    // defining equality of two positions
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    int ManhattanDistanceTo(const Position& other) const {
        return abs(x - other.x) + abs(y - other.y);
    }

    std::string ToString() const {
        return "( " + std::to_string(x) + ", " + std::to_string(y) + " )";
    }
};

#endif