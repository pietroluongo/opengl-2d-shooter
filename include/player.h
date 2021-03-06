#ifndef PLAYER_H
#define PLAYER_H

#include "../include/platform.h"
#include "character.h"

class Player : public Character {
  private:
    void handleMovementKeys();
    double jumpTime = 0;

    bool isRequestingJump = false;
    bool wasRequestingJump = false;
    bool shouldIncreaseHeight = false;
    bool isRequestingMove = false;

    bool isRequestingFire = false;

    bool hasHitHead = false;

  public:
    bool isInvincible = false;

    Player(GLfloat x, GLfloat y, GLfloat size);
    virtual ~Player();
    void draw();
    void idle();
    void applyDamage(int damage) { Character::applyDamage(damage); };
    void updateArmAngle();
    bool checkCollision(Platform* platform);
    void handleJump();
    void shoot();
    void updateAnimState();
    double getJumpTime() { return this->jumpTime; };
    virtual void kill();
    virtual std::vector<std::vector<Collider*>*> colliders();
};

#endif