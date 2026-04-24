#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "constants.h"

class Entity {
public:
  // Конструктор инициализирует базовые параметры
  Entity(int start_gx, int start_gy, const std::string& texturePath) 
    : gx(start_gx), gy(start_gy), sprite(texture) {
        
    if (texture.loadFromFile(texturePath)) {
      texture.setSmooth(true);
      sprite.setTexture(texture, true);
    }
  }
  virtual ~Entity() = default;

  // Чисто виртуальная функция: каждый наследник будет обновляться по-своему
  virtual void update(float dt) = 0;

  // Общая логика отрисовки(всё уже посчитано в entity.h)
  virtual void draw(sf::RenderWindow& window) {
    window.draw(sprite);
  }
  // Расчёт середины низа клетки для отрисовки в будущем
  sf::Vector2f getCenterBottomPos(int x, int y) const {
    return { x * tileSize + (tileSize  / 2.f), (y + 1) * tileSize };
  }
  //Геттеры 
  int getGX() const { return gx; }
  int getGY() const { return gy; }

  //Для боя
  void takeDamage(int amount) { hp -= amount; if (hp < 0) hp = 0; }
  bool isAlive() const { return hp > 0; }

protected:
  int gx, gy;
  sf::Texture texture;
  sf::Sprite sprite;
  sf::Vector2f visualPos;
  float tileSize = Config::Tile::Size;

  // Статы, которые потом переопределим для всех сущностей
  // берем из конфига
  int hp = Config::BaseStats::DefaultHP;
  int maxHp = Config::BaseStats::DefaultHP;
  float damage = Config::BaseStats::DefaultDamage;
};