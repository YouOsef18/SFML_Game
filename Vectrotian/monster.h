#pragma once
#include <SFML/Graphics.hpp>
#include "entity.h"
#include <string>

class Map; // Предварительное объявление

class Monster : public Entity {

public:
  Monster(int gx, int gy, const std::string& texPath, std::string name, 
            int hp, int dmg, int rew, const Map& map);

  virtual void update(float dt) override;

  // Геттеры для логики боя
  int getHP() const { return hp; }
  int getDamage() const { return damage; }
  const std::string& getName() const { return name; }

  //Движение вперёд к точке
  void moveTowards(sf::Vector2f target, float speed, float dt);

protected:
  sf::Vector2f visualPos;  // Текущая плавная позиция спрайта
  const Map& gameMap;
  std::string name;
  int reward;
}; 