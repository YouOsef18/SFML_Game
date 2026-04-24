#include "player.h"
#include "constants.h"
#include <iostream>

// Вызываем конструктор Entity и передаем путь к текстуре мага из конфига
Player::Player(int start_gx, int start_gy) 
  : Entity(start_gx, start_gy, Config::Player::TexIdlePath) {
    
  // Загрузка текстур
  if (!texIdle.loadFromFile(Config::Player::TexIdlePath)) {
    std::cerr << "Error: Could not load " << Config::Player::TexIdlePath << std::endl;
  }
  if (!texAttack.loadFromFile(Config::Player::TexAttackPath)) {
    std::cerr << "Error: Could not load " << Config::Player::TexAttackPath << std::endl;
  }
    
  texWalkDown = texIdle; 
  texWalkUp = texIdle;
  texWalkSide = texIdle;

  setupAnimations();

  // Начальное состояние
  sprite.setTexture(texIdle);
  sprite.setTextureRect(idleFrames[0]);

  // Центрируем Origin сразу
  sf::FloatRect bounds = sprite.getLocalBounds();
  sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y });

  // Инициализация визуальной позиции
  visualPos = getCenterBottomPos(gx, gy);
  sprite.setPosition(visualPos);

  // Назначаем статы из конфига (перезаписываем базовые статы Entity)
  hp = Config::Player::StartHP;
  maxHp = Config::Player::StartHP;
  damage = Config::Player::Damage;
}

void Player::setupAnimations() {
  // Кадры покоя
  idleFrames = {
    sf::IntRect({100, 125}, {141, 183}),
    sf::IntRect({355, 122}, {156, 161}),
    sf::IntRect({620, 120}, {145, 185}),
    sf::IntRect({1130, 120}, {142, 180}),
  };
  // Кадры перемещения в сторону
  walkSideFrames = {
    sf::IntRect({100, 125}, {141, 183}),
    sf::IntRect({355, 122}, {156, 161}),
    sf::IntRect({620, 120}, {145, 185}),
    sf::IntRect({1130, 120}, {142, 180}),
  };
  // Кадры перемещения вперёд
  walkUpFrames = {
    sf::IntRect({100, 125}, {141, 183}),
    sf::IntRect({355, 122}, {156, 161}),
    sf::IntRect({620, 120}, {145, 185}),
    sf::IntRect({1130, 120}, {142, 180}),
  };
  // Кадры перемещения вниз
  walkDownFrames = {
    sf::IntRect({100, 125}, {141, 183}),
    sf::IntRect({355, 122}, {156, 161}),
    sf::IntRect({620, 120}, {145, 185}),
    sf::IntRect({1130, 120}, {142, 180}),
  };
  // Кадры атаки
  attackFrames = {
    sf::IntRect({100, 140}, {155, 180}),
    sf::IntRect({300, 120}, {229, 201}),
    sf::IntRect({860, 110}, {227, 195}),
    sf::IntRect({1115, 110}, {201, 194}),
    sf::IntRect({100, 400}, {257, 213}),
    sf::IntRect({405, 400}, {317, 214}),
    sf::IntRect({800, 400}, {209, 209}),
    sf::IntRect({1075, 400}, {220, 220})
  };
}

void Player::update(float dt) {
  // 1. ПЛАВНОЕ ПЕРЕМЕЩЕНИЕ
  sf::Vector2f targetPos = getCenterBottomPos(gx, gy);

  if (currentState != ATTACK) {
    sf::Vector2f diff = targetPos - visualPos;
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    
    // Считаем размер шага с использованием константы-множителя
    float moveStep = moveSpeed * Config::Player::SpeedMultiplier * dt;

    if (distance > moveStep) {
      currentState = WALK;
      visualPos += (diff / distance) * moveStep;
    } else {
      currentState = IDLE;
      visualPos = targetPos;
    }
  }
    
  // Применяем позицию один раз
  sprite.setPosition(visualPos);
    
  // 2. ЛОГИКА ПОВОРОТА
  if (currentDir == LEFT)  sprite.setScale({-1.0f, 1.0f});
  if (currentDir == RIGHT) sprite.setScale({1.0f, 1.0f});

  // 3. ОБНОВЛЕНИЕ АНИМАЦИИ
  updateAnimation(dt);
}

void Player::updateAnimation(float dt) {
  if (animClock.getElapsedTime().asSeconds() < frameTime) return;

  currentFrame++;
  std::vector<sf::IntRect>* activeFrames = nullptr;

  if (currentState == IDLE) {
    sprite.setTexture(texIdle);
    activeFrames = &idleFrames;
  } else if (currentState == WALK) {
    if (currentDir == UP) { 
      sprite.setTexture(texWalkUp);   
      activeFrames = &walkUpFrames; 
    } else if (currentDir == DOWN) { 
      sprite.setTexture(texWalkDown); 
      activeFrames = &walkDownFrames; 
    } else { 
      sprite.setTexture(texWalkSide); 
      activeFrames = &walkSideFrames; 
    }
  } else if (currentState == ATTACK) {
    sprite.setTexture(texAttack);
    activeFrames = &attackFrames;
    if (currentFrame >= activeFrames->size()) {
      currentState = IDLE;
      currentFrame = 0;
    }
  }

  // Зацикливание (если не атака)
  if (activeFrames && currentFrame >= activeFrames->size()) {
    currentFrame = 0;
  }

  // Финальное применение кадра
  if (activeFrames) {
    sprite.setTextureRect((*activeFrames)[currentFrame]);
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y });
  }

  animClock.restart();
}

void Player::handleInput(sf::Keyboard::Key key, const Map& map, 
                         const std::vector<std::unique_ptr<Monster>>& enemies, 
                         const std::vector<std::unique_ptr<Chest>>& chests) {
    
  // Блокируем управление на время атаки
  if (currentState == ATTACK) return;

  // ПРОВЕРКА ДИСТАНЦИИ
  sf::Vector2f targetPos = getCenterBottomPos(gx, gy);
  float distance = std::sqrt(std::pow(targetPos.x - visualPos.x, 2) + std::pow(targetPos.y - visualPos.y, 2));
  
  // Используем константы из Config
  if (distance > (Config::Tile::Size / Config::Player::InputLockDivisor)) return;

  // Запуск атаки
  if (key == sf::Keyboard::Key::Space) {
    currentState = ATTACK;
    currentFrame = 0;
    animClock.restart();
    return;
  }
    
  int next_gx = gx;
  int next_gy = gy;

  if (key == sf::Keyboard::Key::W) { next_gy--; currentDir = UP; }
  else if (key == sf::Keyboard::Key::S) { next_gy++; currentDir = DOWN; }
  else if (key == sf::Keyboard::Key::A) { next_gx--; currentDir = LEFT; }
  else if (key == sf::Keyboard::Key::D) { next_gx++; currentDir = RIGHT; }
  else return; 
  
  // Проверка коллизий
  bool isWayBlocked = map.isWall(next_gx, next_gy);
  for (const auto& enemy : enemies) {
    if (enemy->getGX() == next_gx && enemy->getGY() == next_gy) isWayBlocked = true;
  }

  if (isWayBlocked) return;
  gx = next_gx;
  gy = next_gy;
}