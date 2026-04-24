#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "constants.h"

// Структуры спавна можно оставить здесь или вынести в отдельный файл structs.h
struct NPCSpawn {
  int x, y;
  char type; 
};

struct SpawnPoint {
  int x, y;
  char type;
};

class Map {
public:
  // Перечисление внутри класса — так имена не будут конфликтовать с другими частями игры
  enum TileType {
    kEmpty = 0,
    kFloor = 1,
    kWall = 2,
    kObstacle = 3 
  };

  Map(float tile_size = Config::Tile::Size);

  bool loadLevel(const std::string& file_path);
  void draw(sf::RenderWindow& window);

  // Геттеры помечаем как const
  bool isWall(int x, int y) const;
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  float getTileSize() const { return tile_size; }

  void setObstacle(int x, int y, bool isBlocked);
  
  // Публичные векторы данных
  std::vector<SpawnPoint> monsterSpawns;
  std::vector<NPCSpawn> npcSpawns;
  std::vector<SpawnPoint> chestSpawns;

private:
  int width;
  int height;
  float tile_size;

  std::vector<int> grid;

  sf::Texture floor_texture;
  sf::Texture wall_texture;
};