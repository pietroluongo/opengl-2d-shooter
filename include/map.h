#ifndef MAP_H
#define MAP_H

#include "platform.h"
#include <vector>

class Map {
  private:
    std::vector<Platform*> platforms;
    std::vector<Collider*> platformColliders;
    glm::fvec4 worldBounds = {0, 0, 0, 0};

  public:
    Map();
    ~Map();
    void addPlatform(Platform* platform);
    void draw();
    void loadArena(char* fileName);
    std::vector<Platform*> getPlatforms();
    glm::fvec4 getWorldBounds();
    std::vector<Collider*>* getPlatformsColliders() {
        return &this->platformColliders;
    };
};

#endif