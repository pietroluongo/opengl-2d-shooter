#include "../include/object.h"
#include "../include/constants.h"
#include "../include/globalCtx.h"

extern GlobalCtx* context;

Object::Object(GLfloat x, GLfloat y, GLfloat size) {
    this->position = {x, y};
    this->size = size;
    this->collider =
        new Collider(x, y, size, size, this, pivotPosition::CENTER);
    this->colliders = context->getGameRef()->getAllObjectColliders();
}

Object::Object() {}
Object::~Object() {}

void Object::moveX(double amount) {
    this->positionDelta.x += (amount * context->getDeltaTime());
};

void Object::moveY(double amount) {
    this->positionDelta.y += (amount * context->getDeltaTime());
};

void Object::setPosition(glfvec2 position) { this->position = position; }

void Object::setPosition(float x, float y) { this->position = {x, y}; }

glfvec2 Object::getPosition() { return this->position; }

void Object::drawAxis() {

    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(DEBUG_CENTER_POINT_SIZE);
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(DEBUG_AXIS_SIZE, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, DEBUG_AXIS_SIZE, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, DEBUG_AXIS_SIZE);
    glEnd();
}

void Object::idle() {
    this->collisionDirections = {false, false, false, false};
    this->isGrounded = false;
    this->collider->idle();

    for (auto otherCollider : this->colliders) {
        if (this->collider->overlaps(otherCollider)) {
            this->collisionDirections =
                this->collider->getOverlapDirection(otherCollider);
            if (this->collisionDirections[0] && this->positionDelta.x < 0) {
                this->positionDelta.x = 0;
            }
            if (this->collisionDirections[1] && this->positionDelta.x > 0) {
                this->positionDelta.x = 0;
            }
            if (this->collisionDirections[3] && this->positionDelta.y > 0) {
                this->positionDelta.y = 0;
            }
            if (this->collisionDirections[2] && this->positionDelta.y < 0) {
                this->positionDelta.y = 0;
            }

            if (this->collisionDirections[3]) {
                this->isGrounded = true;
            }
        }
    }

    if (!this->isGrounded) {
        this->moveY(5);
    }

    this->position += this->positionDelta;
    this->positionDelta = {0, 0};
}

Collider* Object::getCollider() { return this->collider; }
