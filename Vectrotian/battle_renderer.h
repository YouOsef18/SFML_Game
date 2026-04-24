#pragma once
#include <SFML/Graphics.hpp>
#include "battle_manager.h"

class BattleRenderer {
  private:
    sf::RectangleShape uiBox;
    sf::Text mainText;
    sf::Text statsText;

    // Вспомогательные методы 
    void drawLine(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end, sf::Color color);
    sf::Vector2f gridToScreen(sf::Vector2i gridPos, sf::Vector2f offset);

  public:
    BattleRenderer(const sf::Font& font);
    void draw(sf::RenderWindow& window, const BattleManager& manager);
};