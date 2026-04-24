#include "dialogue_manager.h"
#include "npc.h"
#include "player.h"
#include "constants.h"

DialogueManager::DialogueManager(sf::Font& font) : text(font), actionText(font) {
  bg.setFillColor(Config::Dialogue::BgColor);
  bg.setOutlineColor(Config::Dialogue::OutlineColor);
  bg.setOutlineThickness(Config::Dialogue::OutlineThickness);
  bg.setSize({Config::Dialogue::BoxWidth, Config::Dialogue::BoxHeight});

  text.setCharacterSize(Config::Dialogue::MainCharSize);
  text.setFillColor(sf::Color::White);

  actionText.setCharacterSize(Config::Dialogue::ActionCharSize);
}

void DialogueManager::startDialogue(NPC* npc, Player* player) {
  currentNPC = npc;
  currentPlayer = player;
  active = true;
  levelTransitionReady = false;

  currentLineIdx = currentNPC->questGiven ? (int)currentNPC->dialogLines.size() - 1 : 0;
  
  // Обновляем и форматируем текст один раз при старте
  updateWrappedText();
}

void DialogueManager::handleInput(sf::Keyboard::Key key, int currentLevel) {
  if (!active || !currentNPC || !currentPlayer) return;

  if (key == sf::Keyboard::Key::Escape) {
    active = false;
    return;
  }

  bool isLastLine = (currentLineIdx == (int)currentNPC->dialogLines.size() - 1);

  if (isLastLine) {
    currentNPC->questGiven = true;
    auto& req = currentNPC->quest;
    bool canComplete = currentPlayer->hasItems(req.scoutDrops, req.chaserDrops, req.coins);
    
    if ((key == sf::Keyboard::Key::Enter || key == sf::Keyboard::Key::E) && canComplete) {
      currentPlayer->takeItems(req.scoutDrops, req.chaserDrops, req.coins);
      currentNPC->questCompleted = true;

      // ПРОВЕРКА УСЛОВИЯ ПОБЕДЫ
      if (currentLevel == 3) {
        isGameWon = true; 
        currentNPC->dialogLines.push_back("ПОЗДРАВЛЯЮ! Ты спас подземелье! (Нажми ESC для выхода)");
      } 
      else {
        currentNPC->dialogLines.push_back(Config::Dialogue::SuccessQuestLine);
        levelTransitionReady = true; // Только если это не финал
      }
      currentLineIdx++;
      updateWrappedText();
    }
  }
  else if (key == sf::Keyboard::Key::E || key == sf::Keyboard::Key::Space) {
    currentLineIdx++;
    if (currentLineIdx < (int)currentNPC->dialogLines.size()) {
      updateWrappedText();
    }
  }
}

// Вспомогательный метод, чтобы не дублировать логику
void DialogueManager::updateWrappedText() {
  if (currentLineIdx < (int)currentNPC->dialogLines.size()) {
    text.setString(currentNPC->dialogLines[currentLineIdx]);
    wrapText(text, Config::Dialogue::BoxWidth - (Config::Dialogue::Padding * 2.f));
  }
}

void DialogueManager::wrapText(sf::Text& targetText, float maxWidth) {
  std::string rawString = targetText.getString();
  std::string wrapped, word, line;

  for (char c : rawString) {
    if (c == ' ') {
      targetText.setString(line + word + " ");
      if (targetText.getLocalBounds().size.x > maxWidth) {
        wrapped += line + '\n';
        line = word + " ";
      } else {
        line += word + " ";
      }
      word = "";
    } else {
      word += c;
    }
  }
  
  targetText.setString(line + word);
  if (targetText.getLocalBounds().size.x > maxWidth) {
    wrapped += line + "\n" + word;
  } else {
    wrapped += line + word;
  }
  targetText.setString(wrapped);
}

void DialogueManager::draw(sf::RenderWindow& window) {
  if (!active || !currentNPC) return;
  
  sf::Vector2f winSize = static_cast<sf::Vector2f>(window.getSize());

  // Позиционирование окна диалога
  float bgX = (winSize.x - Config::Dialogue::BoxWidth) / 2.f;
  float bgY = winSize.y - Config::Dialogue::BoxHeight - Config::Dialogue::BottomOffset;
  bg.setPosition({bgX, bgY});

  window.draw(bg);

  // Отрисовка основного текста (ВАЖНО: не вызываем setString тут!)
  text.setPosition({bgX + Config::Dialogue::Padding, bgY + Config::Dialogue::Padding});
  window.draw(text);

  // Логика подсказок квеста
  bool isLastLine = (currentLineIdx == (int)currentNPC->dialogLines.size() - 1);
  if (isLastLine && !currentNPC->questCompleted) {
    auto& req = currentNPC->quest;
    bool canComplete = currentPlayer->hasItems(req.scoutDrops, req.chaserDrops, req.coins);
    
    if (canComplete) {
      actionText.setString(Config::Dialogue::ActionGivePrompt);
      actionText.setFillColor(sf::Color::Green);
    } else {
      actionText.setString(Config::Dialogue::ActionMissingPrompt);
      actionText.setFillColor(sf::Color::Red);
    }
    
    // Позиция подсказки чуть ниже основного текста
    actionText.setPosition({bgX + Config::Dialogue::Padding, bgY + Config::Dialogue::BoxHeight - 40.f});
    window.draw(actionText);
  }
}

bool DialogueManager::isActive() const { 
  return active; 
}

void DialogueManager::close() { 
  active = false; 
}

bool DialogueManager::shouldChangeLevel() const { 
  return levelTransitionReady; 
}