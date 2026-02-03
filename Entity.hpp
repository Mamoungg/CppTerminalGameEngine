#pragma once
#include "Vector2.hpp"
#include <string>
#include <vector>
#include <memory>

class Game;

class Entity {
public:
    Vector2 pos;
    char symbol = '?';
    std::string tag;
    bool active = true;

    Entity(int x, int y, char s, std::string t) : pos{x,y}, symbol(s), tag(std::move(t)) {}
    virtual ~Entity() = default;

    virtual void update(Game& game) = 0;

    void render(std::vector<std::string>& screen) const {
        if (!active) return;
        if (pos.y >= 0 && pos.y < (int)screen.size() &&
            pos.x >= 0 && pos.x < (int)screen[0].size()) {
                screen[pos.y][pos.x] = symbol;
            }
    }
};

// Factory Helpers
std::unique_ptr<Entity> makePlayer(int x, int y);
std::unique_ptr<Entity> makeWall(int x, int y);
std::unique_ptr<Entity> makeCoin(int x, int y);
std::unique_ptr<Entity> makePatrolEnemy(int x, int y);
std::unique_ptr<Entity> makeChaserEnemy(int x, int y);