#include "../include/game.h"
#include "../include/globalCtx.h"
#include <algorithm>

extern GlobalCtx* context;

Game::Game() {
    this->player = nullptr;
    this->map = new Map();
    this->cam = new Camera();
}

Game::~Game() {
    printf("deleting game\n");
    if (this->player != nullptr)
        delete (this->player);
    if (this->cam != nullptr)
        delete (this->cam);
    if (this->map != nullptr)
        delete (this->map);
    for (auto enemy : this->enemies) {
        delete (enemy);
    }
    for (auto projectile : this->projectiles) {
        if (projectile != nullptr)
            delete (projectile);
    }
}

glfvec2 Game::getPlayerPosition() { return this->player->getPosition(); }

void Game::draw() {
    this->map->draw();
    this->player->draw();
    for (auto enemy : this->enemies) {
        enemy->draw();
    }
    for (auto projectile : this->projectiles) {
        if (projectile)
            projectile->draw();
    }
    if (this->state == GameState::OVER) {
        this->drawGameOverScreen();
    }
}

Player* Game::getPlayer() { return this->player; }

void Game::idle() {
    if (this->state == GameState::PLAYING) {
        this->player->idle();
        for (auto enemy : this->enemies) {
            enemy->idle();
        }
        for (auto projectile : this->projectiles) {
            if (projectile != nullptr)
                projectile->idle();
        }
    }
    this->cam->idle();
}

void Game::createPlayer(double x, double y, double size) {
    this->player = new Player(x, y, size);
    this->cam->setFollowTarget(this->player);
}

void Game::createEnemy(double x, double y, double size) {
    this->enemies.push_back(new Enemy(x, y, size));
}

Camera* Game::getMainCamera() { return this->cam; }

Map* Game::getMap() { return this->map; }

std::vector<Enemy*> Game::getEnemies() { return this->enemies; }

void Game::createProjectile(float x, float y, float size, float angle,
                            ProjectileType type) {
    this->projectiles.push_back(new Projectile(x, y, size, angle, type));
}

void Game::deleteProjectile(Projectile* projectile) {
    printf("Deleting...\n");
    for (auto it = this->projectiles.begin(); it != this->projectiles.end();) {
        if (*it == projectile) {
            it = this->projectiles.erase(it);
        } else {
            it++;
        }
    }
}

void Game::deleteEnemy(Enemy* enemy) {
    auto it = std::find(this->enemies.begin(), this->enemies.end(), enemy);
    this->enemies.erase(it);
    delete (enemy);
}

std::vector<Collider*> Game::getAllObjectColliders() {
    std::vector<Collider*> colliders;

    for (auto enemy : this->getEnemies()) {
        colliders.push_back(enemy->getCollider());
    }

    for (auto platform : this->getMap()->getPlatforms()) {
        colliders.push_back(platform->getCollider());
    }

    return colliders;
}

void Game::setState(GameState state) { this->state = state; }

void Game::togglePause() {
    if (this->state == GameState::PLAYING) {
        this->state = GameState::PAUSED;
    } else if (this->state == GameState::PAUSED) {
        this->state = GameState::PLAYING;
    }
}

bool Game::canRestart() { return this->state == GameState::OVER; }

void Game::drawGameOverScreen() {
    glPushMatrix();
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glTranslatef(this->cam->getPosition().x, this->cam->getPosition().y, 0);
    glBegin(GL_QUADS);
    glVertex2f(-250.0f, -250.0f);
    glVertex2f(250.0f, -250.0f);
    glVertex2f(250.0f, 250.0f);
    glVertex2f(-250.0f, 250.0f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    glPopMatrix();
    char* tmpString;
    glColor3f(1.0f, 1.0f, 1.0f);
    sprintf(context->textBuffer, "Game over!");
    tmpString = context->textBuffer;
    glRasterPos2f(this->cam->getPosition().x - 2, this->cam->getPosition().y);
    while (*tmpString) {
        glutBitmapCharacter(context->font, *tmpString);
        tmpString++;
    }

    glRasterPos2f(this->cam->getPosition().x - 4,
                  this->cam->getPosition().y + 2);
    sprintf(context->textBuffer, "Press R to restart");
    tmpString = context->textBuffer;
    while (*tmpString) {
        glutBitmapCharacter(context->font, *tmpString);
        tmpString++;
    }
}