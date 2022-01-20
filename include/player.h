#ifndef PLAYER_H
#define PLAYER_H

#include "../include/platform.h"
#include "character.h"

class Player : public Character {
  private:
    void handleMovementKeys();
    void drawChest();
    void drawArm();
    void drawLegs();
    void drawHead();
    bool isJumping = false;
    /**
     * @brief Controls the player's leg rotation
     * [0] - Left leg
     * [1] - Left knee
     * [2] - Right leg
     * [3] - Right knee
     */
    glm::fvec4 legRotation = {25, 35, 20, 15};

  public:
    Player(GLfloat x, GLfloat y, GLfloat size);
    virtual ~Player();
    void draw();
    void idle();
    void applyDamage(int damage) { Character::applyDamage(damage); };
    void updateArmAngle();
    bool checkCollision(Platform* platform);
    void jump();
    void shoot();
    void updateAnimState();
    float* tmp_getSize() { return &this->size; };
    float* getLegRotation() { return &this->legRotation[0]; };
    int nextAnimFrame();
};

#endif