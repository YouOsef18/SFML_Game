#include "cutscene_manager.h"
#include <iostream>
#include <algorithm>
#include "constants.h"

//Конструктор
CutsceneManager::CutsceneManager(sf::Font &font) : currentSlideIdx(0), active(false), subtitle(font), sprite(placeholder) {
  cutsceneView.setSize({Config::Screen::Width, Config::Screen::Height});
  //subtitle.setFont(font);
  subtitle.setCharacterSize(Config::Cutscene::SubtitleSize);
  subtitle.setFillColor(sf::Color::White);
  subtitle.setOutlineColor(sf::Color::Black);
  subtitle.setOutlineThickness(Config::Cutscene::SubtitleOutline);
  subtitle.setPosition(Config::Cutscene::SubtitleOffset);
}

//Добавление слайда
void CutsceneManager::addSlide(const std::string &texturePath, 
                              sf::Vector2f startPos, 
                              sf::Vector2f endPos, 
                              float startZ, float endZ, float dur, 
                              const std::string &subText, 
                              const std::string &audioPath) {
  SlideData data;
  if (data.texture.loadFromFile(texturePath)) {
    data.texture.setSmooth(true);
    data.startCenter = startPos;
    data.endCenter = endPos;
    data.startZoom = startZ;
    data.endZoom = endZ;
    data.duration = dur;
    data.text = subText;
    data.audioPath = audioPath; // Сохраняем путь к аудио
    slides.push_back(data);
  } 
  else {
    std::cerr << "Error: Could not load slide " << texturePath << std::endl;
  }
}

//Запуска катсцены
void CutsceneManager::start() {
  if (!slides.empty()) {
    active = true;
    currentSlideIdx = 0;
    setupCurrentSlide();
  }
}

//Обновление логики камеры и врмени
void CutsceneManager::update(float dt) {
  if (!active || slides.empty()) return;
  SlideData& cur = slides[currentSlideIdx];
  float time = slideClock.getElapsedTime().asSeconds();
  //Отвечает за плавное движение по слайду
  float progress = std::min(Config::Cutscene::DefaultZoom, time / cur.duration);

  sf::Vector2f currenCenter = cur.startCenter + (cur.endCenter - cur.startCenter) * progress;
  cutsceneView.setCenter(currenCenter);

  float currentZoom = cur.startZoom + (cur.endZoom - cur.startZoom) * progress;
  cutsceneView.setSize({Config::Screen::Width * currentZoom, Config::Screen::Height * currentZoom});

  if (time >= cur.duration) {
    next();
  }
}

void CutsceneManager::next() {
  currentSlideIdx++;
  if (currentSlideIdx >= slides.size()) {
    skip(); // Заканчиваем катсцену корректно
  } 
  else {
    setupCurrentSlide();
  }
}

//Пропуск катсцены
void CutsceneManager::skip() {
  active = false;
  slideAudio.stop(); //Глушим звук
}

//Проверка активности
bool CutsceneManager::isActive() const {
  return active;
}

//Отрисовка катсцены
void CutsceneManager::draw(sf::RenderWindow& window) {
  if(!active) return;

  window.setView(cutsceneView);
  window.draw(sprite);

  window.setView(window.getDefaultView());
  if (!slides[currentSlideIdx].text.empty()) {
    window.draw(subtitle);
  }
}

//Внутренняя настройка каждого слайда
void CutsceneManager::setupCurrentSlide() {
  SlideData& cur = slides[currentSlideIdx];
  sprite.setTexture(cur.texture, true);

  sf::FloatRect bounds = sprite.getLocalBounds();
  sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y  / 2.f});
  sprite.setPosition({bounds.size.x  / 2.f, bounds.size.y / 2.f});

  subtitle.setString(cur.text);
  slideClock.restart();

  //Запуск озвучки
  slideAudio.stop(); //Выключаем старый звук
  if (!cur.audioPath.empty()) {
    if (slideAudio.openFromFile(cur.audioPath)){
      slideAudio.play();
    }
    else {
      std::cerr << "Error: Could not load audio " << cur.audioPath << std::endl;
    }
  }
}