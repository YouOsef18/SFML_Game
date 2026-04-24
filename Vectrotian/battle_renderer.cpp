#include "battle_renderer.h"
#include <cmath>

BattleRenderer::BattleRenderer(const sf::Font& font) 
    : mainText(font, ""), statsText(font, "") {
  
  // 1. Настройка боевого окна
  uiBox.setSize({Config::Battle::BoxSize});
  uiBox.setFillColor(Config::Battle::BgColor);
  uiBox.setOutlineThickness(Config::Battle::OutlineThickness);
  uiBox.setOutlineColor(sf::Color::White);
  
  uiBox.setPosition({
    Config::Screen::Width / 2.f - Config::Battle::BoxSize.x / 2.f,
    Config::Screen::Height / 2.f - Config::Battle::BoxSize.y / 2.f
  });

  // 2. Настройка текста
  mainText.setCharacterSize(Config::Battle::MainTextSize);
  mainText.setPosition(uiBox.getPosition() + Config::Battle::MainTextOffset);
}

sf::Vector2f BattleRenderer::gridToScreen(sf::Vector2i gridPos, sf::Vector2f offset) {
  return offset + sf::Vector2f(
    gridPos.x * Config::Battle::CellSize + Config::Battle::CellSize / 2.f,
    gridPos.y * Config::Battle::CellSize + Config::Battle::CellSize / 2.f 
  );
}

void BattleRenderer::drawLine(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end, sf::Color color) {
  sf::Vector2f dir = end - start;
  float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

  sf::RectangleShape line({len, Config::Battle::InlineThickness});
  line.setOrigin(Config::Battle::LineOrigin);
  line.setPosition(start);
  line.setFillColor(color);
    
  float angle = std::atan2(dir.y, dir.x) * Config::Battle::RadTransFactor;
  line.setRotation(sf::degrees(angle)); 
  
  window.draw(line);
}

void BattleRenderer::draw(sf::RenderWindow& window, const BattleManager& manager) {
  if (!manager.isActive()) return;

  // 1. Отрисовка основного окна боя
  window.draw(uiBox);

  // Синхронизируем текст с логикой перед отрисовкой
  mainText.setString(manager.getMessage());
  mainText.setFillColor(manager.getMessageColor());
  window.draw(mainText);

  // Получаем отступ сетки из менеджера
  sf::Vector2f offset = manager.getGridOffset();

  // 3. Отрисовка фоновой сетки
  for (int i = 0; i <= Config::Battle::GridSize; ++i) {
    float step = i * Config::Battle::CellSize;
    float baseOffset = Config::Battle::GridSize * Config::Battle::CellSize;
    
    drawLine(window, offset + sf::Vector2f(step, 0), 
                     offset + sf::Vector2f(step, baseOffset), Config::Battle::GridColor);
    
    drawLine(window, offset + sf::Vector2f(0, step), 
                     offset + sf::Vector2f(baseOffset, step), Config::Battle::GridColor);
  }

  // Получаем данные змейки
  const auto& points = manager.getSnakePoints();
  const auto& input = manager.getPlayerInput();
  BattleState state = manager.getState();

  // 4. Отрисовка Змейки
  if (state == BattleState::SHOW_PATH) {
    for (size_t i = 0; i < points.size() - 1; ++i) {
      drawLine(window, gridToScreen(points[i], offset), 
                       gridToScreen(points[i+1], offset), sf::Color::Yellow);
    }
  }
  else if (state == BattleState::INPUT_PATH || state == BattleState::RESULT) {
    if (!input.empty()) {
      for (size_t i = 0; i < input.size(); ++i) {
        drawLine(window, gridToScreen(points[i], offset), 
                         gridToScreen(points[i+1], offset), sf::Color::Green);
      }
    }
  }
}