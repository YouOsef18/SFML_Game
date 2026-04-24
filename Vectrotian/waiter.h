#pragma once
#include "monster.h"
#include <SFML/Audio.hpp>
#include <optional>

class Player; // Предварительное объявление

class Waiter : public Monster {
public:
  Waiter(int gx, int gy, int hp, int dmg, int rew, const Map& map, const Player& p);

  void update(float dt) override;

private:
  const Player& player;
    
  // Звук
  sf::SoundBuffer soundBuffer;
  std::optional<sf::Sound> greetSound; // Вместо обычного sf::Sound
  bool hasGreeted; 

  // Анимация
  float animTimer;
};