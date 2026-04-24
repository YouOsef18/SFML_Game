#pragma once 
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <string>

// Твои инклуды
#include "map.h"
#include "player.h"
#include "monster.h"
#include "chest.h"
#include "npc.h"
#include "battle_manager.h"
#include "battle_renderer.h"
#include "dialogue_manager.h"
#include "cutscene_manager.h"
#include "monster_factory.h"

class Game {
private:
  // Системные
  sf::RenderWindow window;
  sf::View camera;
  sf::Clock clock;
  sf::Font gameFont;

  // Аудио
  sf::Music backgroundMusic;
  std::vector<std::string> playlist;
  int currentTrackIdx;

  // Все менеджеры — указатели
  std::unique_ptr<CutsceneManager> intro;
  std::unique_ptr<DialogueManager> dialogueManager;
  std::unique_ptr<BattleManager> battleManager;
  std::unique_ptr<BattleRenderer> battleRenderer;

  // Игровой мир
  int currentLevel;
  Map game_map;
  Player player;
  std::vector<std::unique_ptr<NPC>> npcs;
  std::vector<std::unique_ptr<Monster>> enemies;
  std::vector<std::unique_ptr<Chest>> chests;
  MonsterFactory monsterFactory; // Создаем объект фабрики

  // Состояния
  bool isIntroPlaying;

  // Внутренние методы
  void loadResources();
  void initIntro();
  void setupMusic();
  void handleEvents();
  void update(float dt);
  void render();
  void loadLevel(int levelID);
  void checkBattleStart();

public:
  Game();
  void run(); // Главный метод запуска
};