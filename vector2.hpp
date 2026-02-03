#pragma once

struct Vector2 {
    int x = 0;
    int y = 0;

    bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }
};