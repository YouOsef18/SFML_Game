#pragma once
#include <SFML/Graphics.hpp> // Оставляем для sf::Vector2, sf::Clock и sf::Color
#include <vector>
#include <string>
#include "player.h"
#include "monster.h"

enum class BattleState {
  INACTIVE,
  MENU,
  SHOW_PATH,
  INPUT_PATH,
  RESULT,
  ENEMY_TURN,
  ENEMY_RESULT
};

enum class ActionType { HEAL, ATTACK };
enum class BattleDir { Up, Down, Left, Right, None };

class BattleManager {
  private:
    BattleState state = BattleState::INACTIVE;
    ActionType currentAction = ActionType::ATTACK;  
    
    // Моб и игрок
    Player* player = nullptr;
    Monster* enemy = nullptr;
    
    // Сетка и змейка
    sf::Vector2f gridOffset;
    std::vector<BattleDir> pattern;
    std::vector<BattleDir> playerInput;
    std::vector<sf::Vector2i> snakeGridPoints;

    // Переменные отвечающие за время 
    sf::Clock timer;
    float turnTimer = 0.f;

    // Переменные, отвечающие за уворот от атаки моба
    float qteAppearTime = 0.f; 
    bool qteActive = false;    
    bool dodged = false;       
    bool hasAttacked = false; 

    // Состояние текста (вместо sf::Text храним просто данные)
    std::string message;
    sf::Color messageColor = sf::Color::White;

    // Вспомогательные методы
    void generatePattern(int length);
    void finishPlayerTurn(bool success);
    void calculateGridOffset();

  public:
    BattleManager();

    void startBattle(Player& p, Monster& m);
    void update(float dt);
    void handleInput(sf::Keyboard::Key key);

    // Геттеры для Рендерера
    bool isActive() const { return state != BattleState::INACTIVE; }
    BattleState getState() const { return state; }
    const std::vector<sf::Vector2i>& getSnakePoints() const { return snakeGridPoints; }
    const std::vector<BattleDir>& getPlayerInput() const { return playerInput; }
    const std::string& getMessage() const { return message; }
    sf::Color getMessageColor() const { return messageColor; }
    sf::Vector2f getGridOffset() const { return gridOffset; }
};