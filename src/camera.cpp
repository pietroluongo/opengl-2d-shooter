#include "../include/camera.h"
#include "../include/globalCtx.h"
#include "../include/keymap.h"

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef USE_GLUT
#include <GL/glut.h>
#endif

#define BASE_CAMERA_WIDTH 100
#define BASE_CAMERA_HEIGHT 100

constexpr float CAMERA_SPEED = 50.0f;

constexpr float DEFAULT_CAMERA_WIDTH = BASE_CAMERA_WIDTH / 2;
constexpr float DEFAULT_CAMERA_HEIGHT = BASE_CAMERA_HEIGHT / 2;

extern GlobalCtx* context;

Camera::Camera() {}

void Camera::idle() {
    this->handleInput();
    if (this->shouldFollowTarget) {
        this->setCenter(this->followTarget->getPosition());
    }
    this->shouldFollowTarget = !this->freeCamEnabled;
    this->updateBounds();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glOrtho(this->bounds[0], this->bounds[1], this->bounds[2], this->bounds[3],
            -1, 1);
}

glm::fvec4 Camera::getBounds() { return this->bounds; }

void Camera::moveX(float x) { this->center.x += x * context->getDeltaTime(); }
void Camera::moveY(float y) { this->center.y += y * context->getDeltaTime(); }

void Camera::setCenter(glm::fvec2 focus) {
    this->center = focus;
    this->updateBounds();
}

void Camera::updateBounds() {
    float camHeight, camWidth;
    camHeight = this->size.y;
    this->center.y = this->targetYCoordinate;
    // if (this->followMode == CAMERA_FOLLOW_MODE_SINGLE_AXIS) {
    //     camHeight = this->size.y;
    //     this->center.y = this->targetYCoordinate;
    // } else if (this->followMode == CAMERA_FOLLOW_MODE_DUAL_AXIS) {
    //     camHeight = DEFAULT_CAMERA_HEIGHT;
    // } else {
    //     camHeight = DEFAULT_CAMERA_HEIGHT;
    // }
    // if (this->followMode == CAMERA_FOLLOW_MODE_SINGLE_AXIS) {
    //     camWidth = camHeight;
    // } else if (this->followMode == CAMERA_FOLLOW_MODE_DUAL_AXIS) {
    //     camWidth = DEFAULT_CAMERA_WIDTH;
    // } else {
    //     camWidth = DEFAULT_CAMERA_WIDTH;
    // }
    // if (context->getWindowSize().x <= 500) {
    //     camWidth = context->getWindowSize().x / 5;
    // } else {
    //     camWidth = context->getWindowSize().x / 15;
    // }
    if (context->getWindowSize().x >= context->getWindowSize().y) {
        camWidth =
            camHeight * context->getWindowSize().x / context->getWindowSize().y;
    } else {
        camWidth =
            camHeight * context->getWindowSize().y / context->getWindowSize().x;
    }

    this->bounds[0] = center.x - (camWidth * this->zoomLevel);
    this->bounds[1] = center.x + (camWidth * this->zoomLevel);

    this->bounds[2] = center.y + (camHeight * this->zoomLevel);
    this->bounds[3] = center.y - (camHeight * this->zoomLevel);
}

glm::fvec2 Camera::getPosition() { return glm::fvec2(center.x, center.y); }

void Camera::setFollowTarget(Object* target) {
    this->followTarget = target;
    this->shouldFollowTarget = true;
}

void Camera::handleInput() {
    if (this->freeCamEnabled) {
        if (context->isKeyPressed(keymap::MOVE_CAMERA_RIGHT_BUTTON)) {
            this->moveX(CAMERA_SPEED);
        }
        if (context->isKeyPressed(keymap::MOVE_CAMERA_LEFT_BUTTON)) {
            this->moveX(-CAMERA_SPEED);
        }
        if (context->isKeyPressed(keymap::MOVE_CAMERA_UP_BUTTON)) {
            this->moveY(-CAMERA_SPEED);
        }
        if (context->isKeyPressed(keymap::MOVE_CAMERA_DOWN_BUTTON)) {
            this->moveY(CAMERA_SPEED);
        }
    }
}

void Camera::setFollowMode(CameraFollowMode mode) { this->followMode = mode; }

void Camera::setTargetYCoordinates(float y) { this->targetYCoordinate = y; }

void Camera::setDesiredSize(glm::fvec2 size) { this->size = size; }