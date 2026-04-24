#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // Добавили модуль аудио
#include <vector>
#include <string>




struct SlideData {
  sf::Texture texture;
  sf::Vector2f startCenter;
  sf::Vector2f endCenter;
  float startZoom;
  float endZoom;
  float duration;
  std::string text;
  std::string audioPath; // Где хранится аудио слайда
};

class CutsceneManager {
  private:
    sf::Texture placeholder;//Заглушка
    sf::Sprite sprite;
    std::vector<SlideData> slides;
    sf::Text subtitle;
    sf::View cutsceneView;
    int currentSlideIdx;
    bool active;
    sf::Clock slideClock;
    sf::Music slideAudio; //Музкальный плеер для озвучки
    void setupCurrentSlide(); //Приватный метод настройки логики слайда

  public:
    CutsceneManager(sf::Font& font);

    void addSlide(const std::string& texturePath,
                  sf::Vector2f startPos, sf::Vector2f endPos,
                  float startZ, float endZ, float dur,
                  const std::string& subText = "",
                  const std::string& audioPath = "");
    void start();
    void update(float dt);
    void next();
    void skip();
    bool isActive() const ;
    void draw(sf::RenderWindow& window);
};

