#include "../include/player.h"
#include "../include/globalCtx.h"
#include <cmath>
#include <string>
#include <vector>

extern GlobalCtx* context;

float limitArmMovement(float angle);

Player::Player(GLfloat x, GLfloat y, GLfloat size) : Character(x, y, size) {
    this->armPosition = 0.2 * size;
    this->armWidth = 0.04 * size;
    this->armHeight = 0.4 * size;
    this->collider->resize(size * 0.2, size);
}

Player::~Player() { delete (this->collider); }

void Player::draw() {
    glfvec2 position = this->getPosition();
    glPushMatrix();
    glTranslatef(position.x, position.y, 0.0f);
    glColor3f(0.0f, 1.0f, 1.0f);
    this->drawChest();
    glColor3f(1.0f, 1.0f, 1.0f);
    this->drawHead();
    this->drawLegs();
    glColor3f(1.0f, 0.0f, 0.0f);
    this->drawArm();

    if (context->shouldObjectsDrawCoordinateSystem) {
        this->drawAxis();
    }
    glPopMatrix();

    glfvec2 firePosition = {0, 0};
    firePosition.x = position.x - ((this->armHeight + 0.4 * this->size) *
                                   sin(this->armAngle * M_PI / 180));
    firePosition.y = position.y + ((this->armHeight + 0.4 * this->size) *
                                   cos(this->armAngle * M_PI / 180));
    // glPushMatrix();
    // glTranslatef(firePosition.x, firePosition.y, 0.0f);
    // glColor3f(0.2, 0.2, 0.2);
    // glBegin(GL_POLYGON);
    // glVertex2f(-1, -1);
    // glVertex2f(1, -1);
    // glVertex2f(1, 1);
    // glVertex2f(-1, 1);
    // glEnd();
    // glPopMatrix();

    if (context->shouldObjectsDrawColliders)
        this->collider->draw();
}

void Player::idle() {
    this->isRequestingMove = false;
    if (this->getCollisionArr()[3]) {
        this->isGrounded = true;
    }
    this->Object::idle();
    this->handleMovementKeys();
    this->updateAnimState();
    this->handleJump();
    this->updateArmAngle();
    if (this->getPosition().x >=
        context->getGameRef()->getMap()->getWorldBounds()[1] - 1) {
        context->getGameRef()->setState(GameState::WON);
    }
}

void Player::handleMovementKeys() {
    if (context->isKeyPressed('D') || context->isKeyPressed('d')) {
        this->isRequestingMove = true;
        this->moveX(2.5 * this->size);
    }
    if (context->isKeyPressed('A') || context->isKeyPressed('a')) {
        this->isRequestingMove = true;
        this->moveX(-2.5 * this->size);
    }

    if (context->isKeyPressed('w') || context->isKeyPressed('W') ||
        context->getIsPressingRMB()) {
        this->isRequestingJump = true;
    } else {
        this->isRequestingJump = false;
    }

    // if (context->isKeyPressed('s') || context->isKeyPressed('S')) {
    //     this->moveY(20);
    // }
    if (context->getIsPressingLMB()) {
        if (!isRequestingFire) {
            this->shoot();
            isRequestingFire = true;
        }
    } else {
        isRequestingFire = false;
    }
}

float limitArmMovement(float angle) {
    if (angle > -45 && angle < 0) {
        return -45;
    } else if (angle < -135 && angle > -180) {
        return -135;
    } else if (angle < -180 && angle > -225) {
        return -225;
    } else if (angle < 45 && angle > 0) {
        return 45;
    }
    return angle;
}

void Player::updateArmAngle() {
    glm::fvec2 mousePos = context->getWorldSpaceMousePos();
    glfvec2 position = this->getPosition();
    float dy = position.y - mousePos.y;
    float dx = position.x - mousePos.x;
    this->armAngle = atan2(-dy, -dx) * 180 / M_PI - 90;
    this->armAngle = limitArmMovement(this->armAngle);
    if ((this->armAngle >= 45 && this->armAngle <= 90) ||
        this->armAngle <= -225) {
        this->setHeading(LEFT);
    } else {
        this->setHeading(RIGHT);
    }
}

void Player::handleJump() {
    if (this->isRequestingJump) {
        this->wasRequestingJump = true;
    }
    if (this->isGrounded) {
        jumpTime = 0;
        this->wasRequestingJump = false;
        shouldIncreaseHeight = true;
        this->setIsAffectedByGravity(false);
    }
    if (this->jumpTime >= 1) {
        this->setIsAffectedByGravity(true);
    }
    if (!this->isGrounded && !this->isRequestingJump) {
        this->setIsAffectedByGravity(true);
    }
    if (this->wasRequestingJump && !this->isRequestingJump) {
        shouldIncreaseHeight = false;
    }
    if (this->isRequestingJump && this->jumpTime < 1 && shouldIncreaseHeight) {
        this->moveY(-2.5 * this->size);
    }
    this->jumpTime += context->getDeltaTime();
}

void Player::shoot() {
    // glPushMatrix();
    glfvec2 position = this->getPosition();

    glfvec2 firePosition = {0, 0};
    firePosition.x = position.x - ((this->armHeight + 0.4 * this->size) *
                                   sin(this->armAngle * M_PI / 180));
    firePosition.y = position.y + ((this->armHeight + 0.4 * this->size) *
                                   cos(this->armAngle * M_PI / 180));
    context->getGameRef()->createProjectile(firePosition.x, firePosition.y, 0.5,
                                            (90 + this->armAngle) * M_PI / 180,
                                            PROJECTILE_TYPE_PLAYER);
}

void Player::updateAnimState() {
    if (this->isGrounded) {
        if (this->isRequestingMove) {
            this->currentAnimState = AnimState::WALKING;
        } else {

            this->currentAnimState = AnimState::IDLE;
        }
    } else {
        this->currentAnimState = AnimState::JUMPING;
    }
}

void Player::kill() {
    if (this->isInvincible)
        return;
    context->getGameRef()->setState(GameState::OVER);
    this->isAlive = false;
}