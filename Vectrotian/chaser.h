#pragma once
#include "monster.h"
#include <vector>

class Player; // Предварительное объявление

class Chaser : public Monster {
  private:
    const Player& player; // Ссылка на игрока для отслеживание дистанции
    float moveSpeed;

    //Анимация
    std::vector<sf::IntRect> idleFrames;
    std::vector<sf::IntRect> runFrames;
    float frameTimer;
    int currentFrame;
    bool isMoving; //Флаг: двигается моб или стоит
    sf::Texture texWalk;
  public:
    Chaser(int gx, int gy, int hp, int dmg, int rew, float speed, const Map& map, const Player& player);
    void update(float dt) override;
};
