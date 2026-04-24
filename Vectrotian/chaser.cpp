#include "chaser.h"
#include "map.h"
#include "player.h"
#include <cmath>
#include <iostream>

Chaser::Chaser(int gx, int gy, int hp, int dmg, int rew, 
               float speed, const Map& map, const Player& p)
  : Monster(gx, gy, Config::Chaser::TexturePath, "Chaser", hp, dmg, rew, map), 
    player(p), moveSpeed(speed), frameTimer(0.f), currentFrame(0), isMoving(false) {
  
  // Загружаем текстуру через путь из Config
  if (!texWalk.loadFromFile(Config::Chaser::TexturePath)) {
    std::cerr << "Error: Could not load " << Config::Chaser::TexturePath << std::endl;
  }
  
  sprite.setTexture(texWalk);

  // Координаты кадров оставляем тут (это разметка конкретного ассета)
  idleFrames = {
    sf::IntRect({55, 45}, {60, 63})
  };

  runFrames = {
    sf::IntRect({55, 45}, {60, 63}),
    sf::IntRect({205, 45}, {60, 63}),
    sf::IntRect({358, 45}, {60, 63}),
    sf::IntRect({505, 45}, {60, 63})
  };
  
  // Используем масштаб из конфига
  sprite.setScale({Config::Chaser::Scale, Config::Chaser::Scale});
  sprite.setTextureRect(idleFrames[0]);
  
  sf::FloatRect bounds = sprite.getLocalBounds();
  sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y});
  
  visualPos = getCenterBottomPos(gx, gy);
  sprite.setPosition(visualPos);
}

void Chaser::update(float dt) {
  // Логика анимации
  frameTimer += dt;
  if (frameTimer >= Config::Chaser::AnimFrameTime) {
    frameTimer = 0.f;
    currentFrame++;

    auto& activeFrames = isMoving ? runFrames : idleFrames;
    if (currentFrame >= activeFrames.size()) currentFrame = 0;
    
    sprite.setTextureRect(activeFrames[currentFrame]);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ std::floor(bounds.size.x / 2.f), std::floor(bounds.size.y) });
  }
  
  sf::Vector2f targetPos = getCenterBottomPos(gx, gy); 
  moveTowards(targetPos, moveSpeed, dt);

  if (visualPos != targetPos) {
    isMoving = true;
  } else {
    isMoving = false;

    int diffX = player.getGX() - gx;
    int diffY = player.getGY() - gy;

    float gridDist = std::sqrt(static_cast<float>(diffX * diffX + diffY * diffY));

    // Проверка дистанции агро
    if (gridDist <= Config::Chaser::AgroRange && gridDist > 0.0f) {
      int nextX = gx;
      int nextY = gy;

      if (diffX != 0 && !gameMap.isWall(gx + (diffX > 0 ? 1 : -1), gy)) {
        nextX += (diffX > 0 ? 1 : -1);
      } else if (diffY != 0 && !gameMap.isWall(gx, gy + (diffY > 0 ? 1 : -1))) {
        nextY += (diffY > 0 ? 1 : -1);
      }

      if (nextX != gx || nextY != gy) {
        gx = nextX;
        gy = nextY;
        isMoving = true;
      
        // Разворот спрайта
        float currentScaleX = std::abs(sprite.getScale().x);
        if (diffX < 0) sprite.setScale({-currentScaleX, sprite.getScale().y});
        else if (diffX > 0) sprite.setScale({currentScaleX, sprite.getScale().y});
      }
    }
  }
  sprite.setPosition(visualPos);
}