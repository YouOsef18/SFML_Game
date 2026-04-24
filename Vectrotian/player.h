#pragma once
#include "entity.h"
#include "map.h"
#include <memory>
#include "monster.h"
#include <cmath>
#include "chest.h"


class Player : public Entity {
public:
  Player(int start_gx, int start_gy);

  // Обновление логики и физики
  void update(float dt) override;
  void updateAnimation(float dt);

  // Специфическая логика игрока, обработка входа от пользователя
  void handleInput(sf::Keyboard::Key key, const Map& map, const std::vector<std::unique_ptr<Monster>>& enemies, const std::vector<std::unique_ptr<Chest>>& chests);

  //Геттеры
  float getDamage() { return damage; }
  float getHP() { return hp; }
  // Инвентарь
  int getCoins() const { return coins; }
  int getScoutDrops() const { return scoutDrops; }
  int getChaserDrops() const { return chaserDrops; }
  // Мы возвращаем позицию спрайта, которая плавно меняется в update()
  sf::Vector2f getPosition() const { return sprite.getPosition(); }

  //Сеттеры уникальные для игрока
  void heal(int amount) {
    hp += amount;
    if (hp > maxHp) hp = maxHp; // Чтобы лечение не уходило за границы максимального здоровья
  }
  void addLoot(int s, int c, int g) {
    if (s < 0 || c < 0 || g < 0) return; // Защита от "отрицательного" лута
    scoutDrops += s;
    chaserDrops += c;
    coins += g;
  }
  // Сеттер позиции спрайта
  void setPosition(float gx, float gy) {
    this->gx = gx;
    this->gy = gy;
    visualPos = getCenterBottomPos(gx, gy);
    sprite.setPosition(visualPos);
  }

  // Методы для удобного взаимодействия с инветарём
  bool hasItems(int reqScouts, int reqChasers, int reqCoins) const {
    return (scoutDrops >= reqScouts && chaserDrops >= reqChasers && coins >= reqCoins);
  }

  void takeItems(int reqScouts, int reqChasers, int reqCoins) {
    scoutDrops -= reqScouts;
    chaserDrops -= reqChasers;
    coins -= reqCoins;
  }

private:
  // Инвентарь
  int coins = Config::Player::StartCoins;
  int scoutDrops = Config::Player::StartScoutDrops;
  int chaserDrops = Config::Player::StartChaserDrops;


  //Текстуры для разных файлов
  sf::Texture texIdle;
  sf::Texture texWalkDown;
  sf::Texture texWalkUp;
  sf::Texture texWalkSide;
  sf::Texture texAttack;

  // Списки кадров (ручная нарезка)
  std::vector<sf::IntRect> idleFrames;
  std::vector<sf::IntRect> walkDownFrames;
  std::vector<sf::IntRect> walkUpFrames;
  std::vector<sf::IntRect> walkSideFrames;
  std::vector<sf::IntRect> attackFrames;

  //Управление анимацией
  sf::Clock animClock;
  int currentFrame = 0;
  float frameTime = Config::Player::AnimationFrameTime;

  //Состояния и направления
  enum State { IDLE, WALK, ATTACK } currentState = IDLE;
  enum Direction { UP, DOWN, LEFT, RIGHT } currentDir = DOWN;

  //Скорость перемещения
  float moveSpeed = Config::Player::MoveSpeed;

//Инициализация
void setupAnimations();
};