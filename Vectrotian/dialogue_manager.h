#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Предварительное объявление классов
class NPC;
class Player;

class DialogueManager {
public:
  DialogueManager(sf::Font& font);
  
  void resetLevelTransition() { levelTransitionReady = false; }
  void startDialogue(NPC* npc, Player* player);
  void handleInput(sf::Keyboard::Key key, int currentLevel);
  void draw(sf::RenderWindow& window);

  bool isActive() const;
  void close();
  bool shouldChangeLevel() const;
  bool checkWinCondition() const { return isGameWon; }

private:
  // Вспомогательные методы для работы с текстом
  void updateWrappedText();
  void wrapText(sf::Text& text, float maxWidth);

  NPC* currentNPC = nullptr;
  Player* currentPlayer = nullptr;
  bool active = false;
  int currentLineIdx = 0;
  bool levelTransitionReady = false;
  bool isGameWon = false;

  sf::RectangleShape bg;
  sf::Text text;
  sf::Text actionText;
};