#include "map.h"
#include "constants.h"
#include <fstream>
#include <iostream>

Map::Map(float tile_size) : tile_size(tile_size), width(0), height(0) {
  if (!floor_texture.loadFromFile(Config::Map::FloorTexture)) {
    std::cerr << "Error: Could not load ground texture!" << std::endl;
  } else {
    floor_texture.setSmooth(false);
  }

  if (!wall_texture.loadFromFile(Config::Map::WallTexture)) {
    std::cerr << "Error: Could not load wall texture!" << std::endl;
  } else {
    wall_texture.setSmooth(false);
  }
}

bool Map::loadLevel(const std::string& file_path) {
  // Очистка данных перед загрузкой
  grid.clear();
  monsterSpawns.clear();
  npcSpawns.clear();
  chestSpawns.clear();

  std::ifstream file(file_path);
  if (!file.is_open()) return false;

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line)) {
    if (!line.empty() && line.back() == '\r') line.pop_back();
    lines.push_back(line);
  }

  if (lines.empty()) return false;

  height = static_cast<int>(lines.size());
  width = static_cast<int>(lines[0].size());
  grid.assign(width * height, kEmpty);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (x >= static_cast<int>(lines[y].size())) continue;
      
      char c = lines[y][x];
      int index = y * width + x; // Считаем индекс один раз

      // 1. Проверяем на стену
      if (c == Config::Map::Symbols::Wall) {
        grid[index] = kWall;
      } 
      // 2. Группа монстров (все они стоят на обычном полу)
      else if (c == Config::Map::Symbols::Scout || 
              c == Config::Map::Symbols::Chaser || 
              c == Config::Map::Symbols::Waiter) {
        monsterSpawns.push_back({x, y, c});
        grid[index] = kFloor;
      }
      // 3. Группа блокирующих объектов
      else if (c == Config::Map::Symbols::Chest) {
        chestSpawns.push_back({x, y, c});
        grid[index] = kObstacle;
      }
      else if (c == Config::Map::Symbols::NPC) {
        npcSpawns.push_back({x, y, c});
        grid[index] = kObstacle;
      }
      // 4. Всё остальное — пустой пол
      else {
        grid[index] = kFloor;
      }
    }
  }
  return true;
}

void Map::draw(sf::RenderWindow& window) {
  if (height == 0 || width == 0) return;

  sf::Sprite sprite(wall_texture);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int tile = grid[y * width + x];
      if (tile == kEmpty) continue;

      sprite.setTexture(tile == kWall ? wall_texture : floor_texture);

      sf::Vector2u tex_size = sprite.getTexture().getSize();
      float scale_x = tile_size / static_cast<float>(tex_size.x);
      float scale_y = tile_size / static_cast<float>(tex_size.y);
      sprite.setScale({scale_x, scale_y});

      sprite.setPosition({
        static_cast<float>(x) * tile_size, 
        static_cast<float>(y) * tile_size
      });
      
      window.draw(sprite);
    }
  }
}

bool Map::isWall(int x, int y) const {
  if (x < 0 || x >= width || y < 0 || y >= height) return true;
  int tile = grid[y * width + x];
  return (tile == kWall || tile == kObstacle);
}

void Map::setObstacle(int x, int y, bool isBlocked) {
  if (x < 0 || x >= width || y < 0 || y >= height) return;
  grid[y * width + x] = isBlocked ? kObstacle : kFloor;
}