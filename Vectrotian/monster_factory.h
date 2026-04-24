#pragma once
#include <memory>
#include <cstdlib>
#include "monster.h"
#include "scout.h"
#include "chaser.h"
#include "waiter.h"
#include "constants.h"

// Опережающее объявление (Forward declaration)
class Map;
class Player;

class MonsterFactory {
public:
  MonsterFactory() = default;

  std::unique_ptr<Monster> createMonster(char type, int x, int y, Map& map, Player& player) const {
    if (type == Config::Map::Symbols::Scout) {
      return std::make_unique<Scout>(x, y, 
        randomize(Config::Scout::MaxHP), 
        Config::Scout::Damage, 
        randomize(Config::Scout::XpDrop), 
        Config::Scout::MoveSpeed, map);
    } 
    
    if (type == Config::Map::Symbols::Chaser) {
      return std::make_unique<Chaser>(x, y, 
        randomize(Config::Chaser::MaxHP), 
        Config::Chaser::Damage, 
        randomize(Config::Chaser::XpDrop), 
        Config::Chaser::MoveSpeed, map, player);
    } 
    
    if (type == Config::Map::Symbols::Waiter) {
      return std::make_unique<Waiter>(x, y, 
        randomize(Config::Waiter::MaxHP), 
        Config::Waiter::Damage, 
        randomize(Config::Waiter::XpDrop), map, player);
    }
    
    return nullptr;
  }

private:
  // Вынесли общую логику рандома в отдельную функцию
  int randomize(int baseValue) const {
    if (baseValue <= 0) return 0;
    int minVal = baseValue * Config::Monsters::CharRandomFactor;
    return minVal + (std::rand() % (baseValue - minVal + 1));
  }
};