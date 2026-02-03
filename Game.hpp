#pragma once
#include "Entity.hpp"
#include <vector>
#include <memory>
#include <string>

class Player;

class Game {
public:
    Game();

    void run();

    bool isWallAt(const Vector2& p) const;
    Entity* findEntityAt(const Vector2& p, const std::string& tag);
    bool isInside(const Vector2& p) const;

    void setGameOver(bool v) { gameOver = v; }
    void setWin(bool v) { win = v; }

    void addMove() { moves++; }
    int getMoves() const { return moves; }

    void collectCoin();
    int coinsRemaining() const { return coinsLeft; }

    Player* getPlayer() const { return player; }

private:
    void restart();
    void loadLevel();
    void update();
    void render();

    std::vector<std::unique_ptr<Entity>> entities;
    Player* player = nullptr;

    bool running = true;
    bool gameOver = false;
    bool win = false;

    int width = 40;
    int height = 20;

    int moves = 0;
    int coinsLeft = 0;
};