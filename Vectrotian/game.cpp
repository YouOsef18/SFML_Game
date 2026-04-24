#include "game.h"
#include "scout.h"
#include "chaser.h"
#include "waiter.h"
#include "constants.h"
#include <iostream>



Game::Game() : 
  window(sf::VideoMode({Config::Screen::Width, Config::Screen::Height}), Config::Engine::WindowTitle),
  game_map(Config::Tile::Size),
  player(Config::InitialState::PlayerPos1X, Config::InitialState::PlayerPos1Y),
  currentLevel(1),
  isIntroPlaying(true),
  currentTrackIdx(0)
{
  window.setFramerateLimit(Config::Engine::FPS);
  camera.setSize({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
  
  loadResources();

  // Теперь спокойно создаем всех, кому этот шрифт нужен
  intro = std::make_unique<CutsceneManager>(gameFont);
  dialogueManager = std::make_unique<DialogueManager>(gameFont);
  battleManager = std::make_unique<BattleManager>();
  battleRenderer = std::make_unique<BattleRenderer>(gameFont);

  setupMusic();
  initIntro();
  loadLevel(currentLevel);
}

void Game::loadResources() {
  if (!gameFont.openFromFile(Config::Engine::FontPath)) {
    std::cerr << "Error: Could not load font.ttf" << std::endl;
    exit(-1);
  }
}

void Game::setupMusic() {
  playlist = {
    "assets/music/track1.mp3",
    "assets/music/track2.mp3",
    "assets/music/track3.mp3"
  };
  if (backgroundMusic.openFromFile(playlist[currentTrackIdx])) {
    backgroundMusic.setVolume(Config::Audio::MusicVolume);
    backgroundMusic.play();
  }
}

void Game::initIntro() {
  intro->addSlide("assets/intro/intro_1.png", {1000.f, 450.f}, {1100.f, 400.f}, 0.75f, 0.6f, 8.0f, "", "assets/audio/intro_1.mp3");
  intro->addSlide("assets/intro/intro_2.png", {1000.f, 600.f}, {960.f, 540.f}, 0.9f, 1.0f, 11.0f, "", "assets/audio/intro_2.mp3");
  intro->addSlide("assets/intro/intro_3.jpeg", {1000.f, 600.f}, {960.f, 540.f}, 0.9f, 1.0f, 7.0f, "", "assets/audio/intro_3.mp3");
  intro->addSlide("assets/intro/intro_4.png", {1000.f, 550.f}, {960.f, 450.f}, 0.7f, 0.56f, 6.0f, "", "assets/audio/intro_4.mp3");
  intro->addSlide("assets/intro/intro_5.png", {1000.f, 600.f}, {960.f, 540.f}, 0.55f, 0.75f, 8.0f, "", "assets/audio/intro_5.mp3");
  intro->start();
}

void Game::loadLevel(int levelID) {
  std::cout << "Loading Level " << levelID << "..." << std::endl;
  npcs.clear(); enemies.clear(); chests.clear();

  std::string levelPath = "assets/level_" + std::to_string(levelID) + ".txt";
  if (!game_map.loadLevel(levelPath)) return;

  std::vector<std::string> dialogue;
  QuestRequirement requirement = {0, 0, 0};
  std::string npcTexture = "assets/npcLVL1.png";

  switch (levelID) {
    case 1:
      dialogue = Config::InitialState::Lvl1Dialogue;
      requirement = {Config::InitialState::Lvl1Rec[0],Config::InitialState::Lvl1Rec[1],Config::InitialState::Lvl1Rec[2]};
      player.setPosition(Config::InitialState::PlayerPos1X, Config::InitialState::PlayerPos1Y);
      break;
    case 2:
      dialogue = Config::InitialState::Lvl2Dialogue;
      requirement = {Config::InitialState::Lvl2Rec[0],Config::InitialState::Lvl2Rec[1],Config::InitialState::Lvl2Rec[2]};
      npcTexture = "assets/npcLVL1.png";
      player.setPosition(Config::InitialState::PlayerPos2X, Config::InitialState::PlayerPos2Y);
      break;
    case 3:
      dialogue = Config::InitialState::Lvl3Dialogue;
      requirement = {Config::InitialState::Lvl3Rec[0],Config::InitialState::Lvl3Rec[1],Config::InitialState::Lvl3Rec[2]}; 
      npcTexture = "assets/npcLVL1.png";
      player.setPosition(Config::InitialState::PlayerPos3X, Config::InitialState::PlayerPos3Y);
      break;
    default:
      dialogue = Config::InitialState::Base;
      player.setPosition(Config::InitialState::PlayerPos1X, Config::InitialState::PlayerPos1Y);
      break;
  }

  for (const auto& spawn : game_map.npcSpawns) {
    npcs.push_back(std::make_unique<NPC>(spawn.x, spawn.y, npcTexture, dialogue, requirement));
  }
  
  //Спавн мобов с помощью простой фабрики
  for (const auto& spawn : game_map.monsterSpawns) {
    // Вызываем метод у объекта monsterFactory
    auto monster = monsterFactory.createMonster(spawn.type, spawn.x, spawn.y, game_map, player);
    
    if (monster) {
      enemies.push_back(std::move(monster));
    }
  }
  for (const auto& spawn : game_map.chestSpawns) {
    chests.push_back(std::make_unique<Chest>(spawn.x, spawn.y));
    game_map.setObstacle(spawn.x, spawn.y, true);
  }
}

void Game::run() {
  while (window.isOpen()) {
    if (backgroundMusic.getStatus() == sf::Music::Status::Stopped) {
      currentTrackIdx = (currentTrackIdx + 1) % playlist.size();
      if (backgroundMusic.openFromFile(playlist[currentTrackIdx])) backgroundMusic.play();
    }

    float dt = clock.restart().asSeconds();
    handleEvents();
    update(dt);
    render();
  }
}

void Game::handleEvents() {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) window.close();

    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (isIntroPlaying) {
        if (keyPressed->code == sf::Keyboard::Key::Space) { intro->skip(); isIntroPlaying = false; }
        else if (keyPressed->code == sf::Keyboard::Key::Enter) { intro->next(); if (!intro->isActive()) isIntroPlaying = false; }
      }
      else if (dialogueManager->isActive()) { dialogueManager->handleInput(keyPressed->code, currentLevel); }
      else if (battleManager->isActive()) { battleManager->handleInput(keyPressed->code); }
      else {
        player.handleInput(keyPressed->code, game_map, enemies, chests);
        
        if (keyPressed->code == sf::Keyboard::Key::Space) checkBattleStart();

        if (keyPressed->code == sf::Keyboard::Key::E) {
          for (auto& npc : npcs) {
            if (std::abs(player.getGX() - npc->getGX()) <= Config::Engine::InteractRadius 
                && std::abs(player.getGY() - npc->getGY()) <= Config::Engine::InteractRadius) {
              dialogueManager->startDialogue(npc.get(), &player);
            }
          }
          for (auto& chest : chests) {
            if (std::abs(player.getGX() - chest->getGX()) <= Config::Engine::InteractRadius 
                && std::abs(player.getGY() - chest->getGY()) <= Config::Engine::InteractRadius && !chest->isOpened()) {
              chest->interact();
              player.addLoot(0, 0, 1);
              player.heal(Config::Chest::InitHeal);
            }
          }
        }
      } 
    }
  }
}

void Game::checkBattleStart() {
  for (auto& mob : enemies) {
    if (std::abs(player.getGX() - mob->getGX()) <= Config::Engine::InteractRadius 
        && std::abs(player.getGY() - mob->getGY()) <= Config::Engine::InteractRadius) {
      battleManager->startBattle(player, *mob);
      break;
    }
  }
}

void Game::update(float dt) {
  if (dialogueManager->checkWinCondition() && !dialogueManager->isActive()) {
    std::cout << "Congratulations! You won the game!" << std::endl;
    // Можно либо закрыть окно, либо поставить флаг isVictoryState = true
    window.close(); 
    return;
  }
  if (player.getHP() <= 0) {
    std::cout << "GAME OVER! Player is dead." << std::endl;
    window.close(); // Самый простой вариант — закрыть игру
    return;
  }

  if (isIntroPlaying) {
    intro->update(dt);
    if (!intro->isActive()) isIntroPlaying = false;
  }
  else if (dialogueManager->shouldChangeLevel()) {
    dialogueManager->resetLevelTransition();
    dialogueManager->close();
    currentLevel++;
    loadLevel(currentLevel);
  }
  else if (battleManager->isActive()) {
    battleManager->update(dt);
    if (!battleManager->isActive()) {
      enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::unique_ptr<Monster>& m) { return not(m->isAlive()); }), enemies.end());
    }
  }
  else {
    player.update(dt);
    for (auto& mob : enemies) {
      mob->update(dt);
      if (player.getGX() == mob->getGX() && player.getGY() == mob->getGY()) {
        battleManager->startBattle(player, *mob);
      }
    }
    sf::Vector2f playerPos = player.getPosition();
    camera.setCenter({ std::round(playerPos.x), std::round(playerPos.y) });
  }
}

void Game::render() {
  window.clear(sf::Color(Config::Engine::BgColor));

  if (isIntroPlaying) {
    intro->draw(window); 
  } else {
    window.setView(camera);
    game_map.draw(window);
    for (auto& mob : enemies) mob->draw(window);
    for (auto& chest : chests) chest->draw(window);
    for (auto& npc : npcs) npc->draw(window);
    player.draw(window);
    
    window.setView(window.getDefaultView()); 
    if (dialogueManager->isActive()) dialogueManager->draw(window);
    if (battleManager->isActive()) battleRenderer->draw(window, *battleManager);
  }
  window.display();
}