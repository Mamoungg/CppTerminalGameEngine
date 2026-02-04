#include "Entity.hpp"
#include "Game.hpp"
#include "Input.hpp"
#include <cstdlib>
#include <cmath>

// WALL
class Wall : public Entity {
public:
    Wall(int x, int y) : Entity(x, y, '#', "Wall") {}
    void update(Game&) override {}
};

// COIN
class Coin : public Entity {
public:
    Coin(int x, int y) : Entity(x, y, '$', "Coin") {}
    void update(Game&) override {}
};

// PLAYER
class Player : public Entity {
public:
    Player(int x, int y) : Entity(x, y, 'P', "Player") {}

    void update(Game& game) override {
        char c = readKey();
        int dx = 0, dy = 0;

        if (c == 'q') std::exit(0);
        if (c == 'w') dy = -1;
        if (c == 's') dy = 1;
        if (c == 'a') dx = -1;
        if (c == 'd') dx = 1;

        if (dx == 0 && dy == 0) return;

        Vector2 next {pos.x + dx, pos.y + dy};
        if (!game.isInside(next)) return;
        if (game.isWallAt(next)) return;

        // Enemy Collision
        if (game.findEntityAt(next, "Enemy")) {
            game.setGameOver(true);
            return;
        }

        Entity* coin = game.findEntityAt(next, "Coin");
        if (coin) {
            coin->active = false;
            game.collectCoin();
        }

        pos = next;
        game.addMove();
    }
};

// PATROL ENEMY
class PatrolEnemy : public Entity {
    int dir = 1;
public:
    PatrolEnemy(int x, int y) : Entity(x, y, 'E', "Enemy") {}

    void update(Game& game) override {
        Vector2 next {pos.x + dir, pos.y};
        if (!game.isInside(next) || game.isWallAt(next)) {
            dir *= -1;
            return;
        }

        if (game.getPlayer() && game.getPlayer()->pos == next) {
            game.setGameOver(true);
            return;
        }

        pos = next;
    }
};

// CHASER ENEMY
class ChaserEnemy : public Entity {
public:
    ChaserEnemy(int x, int y) : Entity(x, y, 'C', "Enemy") {}

    void update(Game& game) override {
        Player* p = game.getPlayer();
        if (!p) return;

        int dx = p->pos.x - pos.x;
        int dy = p->pos.y = pos.y;

        // Only chase if within range
        const int range = 8;
        if (std::abs(dx) + std::abs(dy) > range) return;

        Vector2 step = pos;
        if (std::abs(dx) > std::abs(dy)) step.x += (dx > 0 ? 1 : -1);
        else step.y += (dy > 0 ? 1 : -1);

        if (!game.isInside(step) || game.isWallAt(step)) return;

        if (p->pos == step) {
            game.setGameOver(true);
            return;
        }

        pos = step;
    }
};

// FACTORIES
std::unique_ptr<Entity> makePlayer(int x, int y) { return std::make_unique<Player>(x, y); }
std::unique_ptr<Entity> makeWall(int x, int y) { return std::make_unique<Wall>(x, y); }
std::unique_ptr<Entity> makeCoin(int x, int y) { return std::make_unique<Coin>(x, y); }
std::unique_ptr<Entity> makePatrolEnemy(int x, int y) { return std::make_unique<PatrolEnemy>(x, y); }
std::unique_ptr<Entity> makeChaserEnemy(int x, int y) { return std::make_unique<ChaserEnemy>(x, y); }