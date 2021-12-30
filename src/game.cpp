#include "../include/game.h"

Game::Game() {
    this->player = NULL;
    this->map = new Map();
    this->cam = new Camera();
}

Game::~Game() {
    if (this->player != NULL)
        delete (this->player);
    if (this->cam != NULL)
        delete (this->cam);
    if (this->map != NULL)
        delete (this->map);
}

glfvec2 Game::getPlayerPosition() { return this->player->getPosition(); }

void Game::draw() {
    this->player->draw();
    this->map->draw();
}

Player* Game::getPlayer() { return this->player; }

void Game::idle() {
    this->player->idle();
    this->cam->idle();
}

void Game::createPlayer(double x, double y, double size) {
    this->player = new Player(x, y, size);
    this->cam->setFollowTarget(this->player);
}

Camera* Game::getMainCamera() { return this->cam; }

Map* Game::getMap() { return this->map; }