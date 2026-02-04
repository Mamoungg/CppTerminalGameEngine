#include "Game.hpp"
#include "Input.hpp"
#include <iostream>
#include <thread>
#include <chrono>

Game::Game() {
    restart();
}

void Game::restart() {
    entities.clear();
    player = nullptr;
    gameOver = false;
    win = false;
    moves = 0;
    coinsLeft = 0;
    loadLevel();
}

void Game::collectCoin() {
    if (coinsLeft > 0) coinsLeft--;
    if (coinsLeft == 0) win = true;
}

bool Game::isInside(const Vector2& p) const {
    return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
}

bool Game::isWallAt(const Vector2& p) const {
    for (const auto& e : entities) {
        if (e->active && e->tag == "Wall" && e->pos == p) return true;
    }
    return false;
}

Entity* Game::findEntityAt(const Vector2& p, const std::string& tag) {
    for (auto& e : entities) {
        if (e->active && e->tag == tag && e->pos == p) return e.get();
    }
    return nullptr;
}

void Game::loadLevel() {
    // Simple ASCII Level (You can customized this)
    // # = wall
    // P = Player
    // $ = Coin
    // E = patrol enemy
    // C = chaser enemy
    const std::vector<std::string> level = {
        "########################################",
        "#P     #        $        #            ##",
        "#      #                 #   ######    #",
        "#      #     #######     #            ##",
        "#      #                 #             #",
        "#      #######     #######    E        #",
        "#                        #             #",
        "#   $                    #   ######    #",
        "#                        #             #",
        "#     ######        ######             #",
        "#               $                     ##",
        "#                        #      C      #",
        "#   ######               #######       #",
        "#                        #             #",
        "#        $               #             #",
        "#                        #             #",
        "#                 E                    #",
        "#                                      #",
        "#                               $      #",
        "########################################"
    };

    height = (int)level.size();
    width = (int)level[0].size();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            char c = level[y][x];
            if (c == '#') entities.push_back(makeWall(x, y));
            else if (c == 'P') {
                auto p = makePlayer(x, y);
                player = (Player*)p.get();
                entities.push_back(std::move(p));
            }
            else if (c == '$') { entities.push_back(makeCoin(x, y)); coinsLeft++; }
            else if (c == 'E') entities.push_back(makePatrolEnemy(x, y));
            else if (c == 'C') entities.push_back(makeChaserEnemy(x, y));
        }
    }

    // Safety: if no player placed
    if (!player) {
        auto p = makePlayer(1, 1);
        player = (Player*)p.get();
        entities.push_back(std::move(p));
    }
}

void Game::run() {
    while (running) {
        update();
        render();
        std::this_thread::sleep_for(std::chrono::milliseconds(90));
    }
}

void Game::update() {
    if (win || gameOver) {
        if (readKey() != 0) {
            restart();
        }
        return;
    }

    for (auto& e : entities) {
        if (e->active) e->update(*this);
    }
}

void Game::render() {
    std::cout << "\033[2J\033[1;1H";

    if (win) {
        std::cout << "\n\n\tYOU WIN! Moves: " << moves << "\n\tPress any key to restart...\n";
        return;
    }
    if (gameOver) {
        std::cout << "\n\n\tGAME OVER Moves: " << moves << "\n\tPress any key to restart...\n";

        return;
    }

    std::vector<std::string> screen(height, std::string(width, ' '));
    for (auto& e : entities) e->render(screen);

    std::cout << "TERMINAL GAME | WASD move | Q quit" << " | Moves: " << moves << " | Coins left: " << coinsLeft << "\n";

    for (const auto& line : screen) std::cout << line << "\n";
}