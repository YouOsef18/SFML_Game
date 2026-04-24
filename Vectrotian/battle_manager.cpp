#include "battle_manager.h"
#include <iostream>
#include <cmath>

BattleManager::BattleManager() {
  calculateGridOffset();
}

void BattleManager::calculateGridOffset() {
  // Вычисляем отступ сетки только один раз (без UI элементов)
  sf::Vector2f boxPos = {
    Config::Screen::Width / 2.f - Config::Battle::BoxSize.x / 2.f,
    Config::Screen::Height / 2.f - Config::Battle::BoxSize.y / 2.f
  };
  sf::Vector2f boxCenter = boxPos + (Config::Battle::BoxSize / 2.f);
  gridOffset = boxCenter - (Config::Battle::GridVisualSize / 2.f);
}

void BattleManager::generatePattern(int length) {
  pattern.clear();
  snakeGridPoints.clear();
  playerInput.clear();

  // Старт в центре поля
  sf::Vector2i currentPos(Config::Battle::GridSize / 2, Config::Battle::GridSize / 2);
  snakeGridPoints.push_back(currentPos);
  BattleDir lastDir = BattleDir::None;
  
  for (int i = 0; i < length; ++i) {
    std::vector<BattleDir> possible;
    if (lastDir != BattleDir::Down && currentPos.y > 0) 
      possible.push_back(BattleDir::Up);
    if (lastDir != BattleDir::Up && currentPos.y < Config::Battle::GridSize - 1) 
      possible.push_back(BattleDir::Down);
    if (lastDir != BattleDir::Left && currentPos.x < Config::Battle::GridSize - 1) 
      possible.push_back(BattleDir::Right);

    if (possible.empty()) break;

    BattleDir next = possible[rand() % possible.size()];
    if (next == BattleDir::Up)         currentPos.y--;
    else if (next == BattleDir::Down)  currentPos.y++;
    else if (next == BattleDir::Right) currentPos.x++;

    pattern.push_back(next);
    snakeGridPoints.push_back(currentPos);
    lastDir = next;
  }
}

void BattleManager::handleInput(sf::Keyboard::Key key) {
  if (state == BattleState::MENU) {
    if (key == sf::Keyboard::Key::Num1) { 
      generatePattern(Config::Battle::FightDifficulty); 
      currentAction = ActionType::HEAL;
      state = BattleState::SHOW_PATH;
      message = "MEMORIE!";
      timer.restart();
    }
    else if (key == sf::Keyboard::Key::Num2) { 
      generatePattern(Config::Battle::FightDifficulty); 
      currentAction = ActionType::ATTACK;
      state = BattleState::SHOW_PATH;
      message = "MEMORIE!";
      timer.restart();
    }
  }
  else if (state == BattleState::INPUT_PATH) {
    BattleDir pressedDir = BattleDir::None;

    if (key == sf::Keyboard::Key::Up)         pressedDir = BattleDir::Up;
    else if (key == sf::Keyboard::Key::Down)  pressedDir = BattleDir::Down;
    else if (key == sf::Keyboard::Key::Left)  pressedDir = BattleDir::Left;
    else if (key == sf::Keyboard::Key::Right) pressedDir = BattleDir::Right;

    if (pressedDir != BattleDir::None) {
      if (pressedDir == pattern[playerInput.size()]) {
        playerInput.push_back(pressedDir); 
        if (playerInput.size() == pattern.size()) {
          finishPlayerTurn(true); 
        }
      }
      else {
        finishPlayerTurn(false); 
      }
    }
  }
  else if (state == BattleState::ENEMY_TURN) {
    if (qteActive && key == sf::Keyboard::Key::Space) {
      dodged = true;
      qteActive = false; 

      messageColor = sf::Color::Green;
      message = "DODGED!";
      
      timer.restart();
    }
  }
}
  
void BattleManager::finishPlayerTurn(bool success) {
  state = BattleState::RESULT;
  timer.restart(); 

  int multiplier = playerInput.size(); 

  if (currentAction == ActionType::ATTACK) {
    int finalDamage = player->getDamage() * multiplier;
    
    if (success) {
      messageColor = sf::Color::Green;
      message = "SUCCESS!\nDamage: " + std::to_string(finalDamage);
    } 
    else {
      messageColor = sf::Color::Red;
      message = "FAILED!\nDamage x" + std::to_string(multiplier) + ": " + std::to_string(finalDamage);
    }
    enemy->takeDamage(finalDamage);
  }
  else if (currentAction == ActionType::HEAL) {
    int totalHeal = Config::Battle::BaseHeal + (multiplier * Config::Battle::HealMultiplier);

    if (success) {
      messageColor = sf::Color::Green;
      message = "SUCCESS!\nHealed: " + std::to_string(totalHeal) + " HP";
    } 
    else {
      messageColor = sf::Color::Red;
      message = "FAILED!\nHealed x" + std::to_string(multiplier) + ": " + std::to_string(totalHeal) + " HP";
    }
    player->heal(totalHeal); 
  }
}

void BattleManager::update(float dt) {
  if (state == BattleState::SHOW_PATH) {
    if (timer.getElapsedTime().asSeconds() >= Config::Battle::ShowPathTime) {
      state = BattleState::INPUT_PATH;
      turnTimer = Config::Battle::FightDifficulty / 2.f;
    }
  }
  else if (state == BattleState::RESULT) {
    if (timer.getElapsedTime().asSeconds() >= Config::Battle::ResultWaitTime) {
      // Если игрок погиб во время хода врага
      if (player->getHP() <= 0) {
        state = BattleState::INACTIVE; // Выходим из режима боя
        // Здесь можно добавить флаг isGameOver = true для класса Game
      }
      if (enemy->getHP() <= 0) {
        state = BattleState::INACTIVE; 
        if (enemy->getName() == "Scout") { player->addLoot(1,0,0); }
        else if (enemy->getName() == "Chaser") { player->addLoot(0,1,0); }
      } 
      else {
        state = BattleState::ENEMY_TURN;
        messageColor = sf::Color::Yellow;
        message = enemy->getName() + " is preparing to attack...";
        timer.restart();

        hasAttacked = false;
        dodged = false;
        qteActive = false;

        qteAppearTime = Config::Battle::QteMinWait + static_cast<float>(rand() % Config::Battle::QteRandomRange / 100.f);
      }
    }
  }
  else if (state == BattleState::INPUT_PATH) {
    turnTimer -= dt; 
    message = "REPEAT!\nTime: " + std::to_string(static_cast<int>(turnTimer + 0.9f)); 
    if (turnTimer <= 0.0f) {
      finishPlayerTurn(false); 
    }
  }
  else if (state == BattleState::ENEMY_TURN) {
    float elapsed = timer.getElapsedTime().asSeconds();

    if (dodged) {
      messageColor = sf::Color::Green;
      message = "DODGED!";
        
      if (timer.getElapsedTime().asSeconds() >= Config::Battle::DodgeResultTime) {
        state = BattleState::ENEMY_RESULT;
        timer.restart();
      }
      return; 
    }

    if (elapsed < Config::Battle::EnemyAttackStart) {
      if (elapsed >= qteAppearTime && elapsed <= qteAppearTime + Config::Battle::TimeToDodge) {
        qteActive = true;
        messageColor = sf::Color::Green;
        message = "SPACE!";
      } 
      else if (!qteActive) {
        qteActive = false;
        messageColor = sf::Color::Yellow;
        message = enemy->getName() + " is preparing to attack...";
      }
      else {
        messageColor = sf::Color::Red;
        message = enemy->getName() + " has attacked you!";
      }
    }
    else if (elapsed < Config::Battle::EnemyAttackEnd) {
      if (!hasAttacked) {
        hasAttacked = true;
        qteActive = false; 
        player->takeDamage(enemy->getDamage());
      }
    }
    else if (elapsed < Config::Battle::EnemyInfoEnd) {
      messageColor = sf::Color::Red;
      message = enemy->getName() + " has attacked you!\n" +
                "You took " + std::to_string(enemy->getDamage()) + " damage!";
    }   
    else {
      state = BattleState::ENEMY_RESULT;
      timer.restart();
    }
  }
  else if (state == BattleState::ENEMY_RESULT) {
    if (timer.getElapsedTime().asSeconds() >= Config::Battle::ResultWaitTime) {
      state = BattleState::MENU;
      messageColor = sf::Color::White;
      message = "Choose option:\n1) Heal\n2) Attack\n HP " + std::to_string(static_cast<int>(player->getHP()));
    }
  }
}

void BattleManager::startBattle(Player& p, Monster& m) {
  player = &p; 
  enemy = &m;  
  
  dodged = false;
  hasAttacked = false;
  qteActive = false;

  state = BattleState::MENU;
  messageColor = sf::Color::White; 
  message = "Choose option:\n1) Heal\n2) Attack\n HP " + std::to_string(static_cast<int>(player->getHP()));
}