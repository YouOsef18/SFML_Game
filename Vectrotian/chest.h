#pragma once
#include "entity.h"
class Chest : public Entity {
private:
  sf::Texture texOpen;
  bool isOpen = false;
  

public:
  // Только объявление конструктора
  Chest(int start_gx, int start_gy);

  // Обязательный метод из Entity
  void update(float dt) override;

  // Логика взаимодействия
  void interact();

  bool isOpened() const { return isOpen; }
};