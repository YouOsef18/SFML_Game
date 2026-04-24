#pragma once
#include "entity.h"
#include <vector>
#include <string>

//Структура того, что нужно принести
struct QuestRequirement {
  int scoutDrops;
  int chaserDrops;
  int coins;
};

class NPC : public Entity {
  public:
    NPC(int gx, int gy, const std::string& textPath, std::vector<std::string> lines, QuestRequirement req)
        : Entity(gx, gy, textPath), dialogLines(lines), quest(req), questGiven(false), questCompleted(false)
    {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y});
  
    // Начальная визуальная позиция
    visualPos = getCenterBottomPos(gx, gy);
    sprite.setPosition(visualPos);
    sprite.setScale(Config::NPC::NpcScale);
    }
    void update(float dt) override{};

    std::vector<std::string> dialogLines;
    QuestRequirement quest;

    //Состояния диалога
    bool questGiven;
    bool questCompleted;
};