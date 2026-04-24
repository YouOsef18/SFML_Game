#include "scout.h"
#include "map.h"
#include "constants.h"
#include <cmath>

Scout::Scout(int gx, int gy, int hp, int dmg, int rew, float speed, const Map& map)
  : Monster(gx, gy, Config::Scout::TexturePath, "Scout", hp, dmg, rew, map), 
    moveSpeed(speed), animTimer(0.f) {
  
  currentDir = {1, 0}; 
  sprite.setColor(Config::Scout::ColorModifier); // Зелёный разведчик из конфига
}

void Scout::update(float dt) {
  sf::Vector2f targetPos = getCenterBottomPos(gx, gy);
  moveTowards(targetPos, moveSpeed, dt);

  // Логика поворота и анимации (покачивание при ходьбе)
  if (visualPos != targetPos) {
    animTimer += dt * Config::Scout::AnimSpeedMultiplier;
    float angle = std::sin(animTimer) * Config::Scout::MaxRotationAngle;
    sprite.setRotation(sf::degrees(angle));
  } else {
    // Стоим в центре клетки
    sprite.setRotation(sf::degrees(0));
      
    int nx = gx + currentDir.x;
    int ny = gy + currentDir.y;

    if (!gameMap.isWall(nx, ny)) {
      gx = nx; 
      gy = ny;
    } else {
      // Выбор случайного направления при столкновении со стеной
      sf::Vector2i dirs[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
      currentDir = dirs[std::rand() % Config::Scout::NumDirections];
    }
  }

  // Синхронизируем спрайт с visualPos (обновляется в moveTowards)
  sprite.setPosition(visualPos);
}