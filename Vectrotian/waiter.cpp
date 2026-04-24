#include "waiter.h"
#include "player.h"
#include "constants.h"
#include <iostream>
#include <cmath>

Waiter::Waiter(int gx, int gy, int hp, int dmg, int rew, 
               const Map& map, const Player& p)
  : Monster(gx, gy, Config::Waiter::TexturePath, "Waiter", hp, dmg, rew, map),
    player(p), 
    hasGreeted(false), 
    animTimer(0.f) {

  // Загрузка звука из конфига
  if (soundBuffer.loadFromFile(Config::Waiter::GreetSoundPath)) {
    greetSound.emplace(soundBuffer);
  } else {
    std::cerr << "Error: Could not load " << Config::Waiter::GreetSoundPath << std::endl;
  }
}

void Waiter::update(float dt) {
  // --- Анимация "дыхания" ---
  animTimer += dt * Config::Waiter::BreathSpeed;
  float breath = std::sin(animTimer) * Config::Waiter::BreathAmplitude;
    
  // Вычисляем базовый масштаб (используем AutoScaleFactor из базового класса монстров)
  float baseScale = (Config::Tile::Size * Config::Monsters::AutoScaleFactor) / sprite.getLocalBounds().size.x;
  
  // Применяем пульсацию
  sprite.setScale({baseScale, baseScale + breath});

  // --- Логика приветствия ---
  int dx = std::abs(player.getGX() - gx);
  int dy = std::abs(player.getGY() - gy);

  // Используем радиус из конфига
  if (dx <= Config::Waiter::GreetRadius && dy <= Config::Waiter::GreetRadius) {
    if (!hasGreeted) {
      if (greetSound.has_value()) {
        greetSound->play();
      }
      hasGreeted = true;
    }
  } else {
    // Сбрасываем флаг, когда игрок уходит за пределы радиуса
    hasGreeted = false;
  }

  // Синхронизируем позицию
  sprite.setPosition(visualPos);
}