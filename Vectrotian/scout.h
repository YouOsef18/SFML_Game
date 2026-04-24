#pragma once
#include "monster.h"

class Scout : public Monster {
private:
    sf::Vector2i currentDir;
    float moveSpeed;
    float animTimer;
public:
  Scout(int gx, int gy, int hp, int dmg, int rew, float speed, const Map& map);
  void update(float dt) override;
};