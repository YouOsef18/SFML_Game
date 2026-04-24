#include "monster.h"
#include "map.h"

Monster::Monster(int gx, int gy, const std::string& texPath, std::string name, 
                 int hp_val, int dmg_val, int rew, const Map& map) 
    : Entity(gx, gy, texPath), name(name), reward(rew), gameMap(map) {
  // Поля родителя Entity инициализируем внутри тела
  this->hp = hp_val; 
  this->maxHp = hp_val;
  this->damage = dmg_val;
  // 1. Центрируем Origin по ногам (центр по X, низ по Y)
  sf::FloatRect bounds = sprite.getLocalBounds();
  sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y});

  // 2. Устанавливаем начальную визуальную позицию (центр-низ клетки)
  visualPos = { gx * tileSize + (tileSize / 2.f), gy * tileSize + tileSize };
  sprite.setPosition(visualPos);

  // 3. Авто-масштабирование
  // Берем размер напрямую из локальных границ спрайта
  if (bounds.size.x > 0) {
    float scale = (tileSize * Config::Monsters::AutoScaleFactor) / bounds.size.x;
    sprite.setScale({scale, scale});
  }
}
void Monster::update(float dt) {
  // Базовый монстр просто стоит, но обновляет позицию спрайта
  sprite.setPosition(visualPos);
}

// Стандартная логика перемещения для монстров
void Monster::moveTowards(sf::Vector2f target, float speed, float dt) {
  sf::Vector2f diff = target - visualPos;
  float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

  if (dist > Config::Monsters::ArrivalThreshold) {
    visualPos += (diff / dist) * speed * dt;
  } 
  else {
    visualPos = target;
  }
}