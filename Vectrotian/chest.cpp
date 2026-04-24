#include "chest.h"
#include "constants.h"
#include <iostream>

Chest::Chest(int start_gx, int start_gy) 
  : Entity(start_gx, start_gy, Config::Chest::TexClosedPath) {
    
  if (texture.getSize().x == 0) {
    std::cerr << "!!! ALERT: Entity texture is EMPTY for chest at " << start_gx << ", " << start_gy << std::endl;
  }

  if (!texOpen.loadFromFile(Config::Chest::TexOpenPath)) {
    std::cerr << "Error, couldn't load " << Config::Chest::TexOpenPath << std::endl;
  }

  // Принудительно устанавливаем текстуру закрытого сундука
  sprite.setTexture(texture, true); 
  
  // Масштаб из конфига
  sprite.setScale({Config::Chest::Scale, Config::Chest::Scale}); 

  // Центрируем Origin по нижней кромке
  sf::FloatRect bounds = sprite.getLocalBounds();
  sprite.setOrigin({
    bounds.size.x / 2.f, 
    bounds.size.y + Config::Chest::OriginOffset
  });

  // Позиционирование
  visualPos = getCenterBottomPos(gx, gy);
  sprite.setPosition(visualPos);
} 

void Chest::update(float dt) {
  // Сундуки статичны
}

void Chest::interact() {
  if (!isOpen) {
    isOpen = true;
    sprite.setTexture(texOpen, true);
    
    // После смены текстуры стоит пересчитать Origin, 
    // если размеры открытого и закрытого сундука разные
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({
      bounds.size.x / 2.f, 
      bounds.size.y + Config::Chest::OriginOffset
    });
  }
}