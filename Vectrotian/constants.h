#pragma once
#include "string"
#include <vector>

//Используем inline, чтобы сложные объекты сохранялись только 1 раз

namespace Config {

  namespace Screen {
    const unsigned int Width = 1920;
    const unsigned int Height = 1080;
  }

  namespace Engine {
    inline const std::string WindowTitle = "Vectrotian: Map Prototype";
    inline const std::string FontPath = "assets/font.ttf";
    const unsigned int FPS = 60;
    const sf::Color BgColor = sf::Color(240, 230, 200);
    const int InteractRadius = 1; // Радиус взаимодействия с NPC/Сундуками
  }
  namespace Audio {
    const float MusicVolume = 5.f;
  }
  
  //Позиция игрока и запрос к уровню
  namespace InitialState {
    const int StartLevel = 1;
    //ЛВЛ1
    const int PlayerPos1X = 2;
    const int PlayerPos1Y = 2;
    const std::vector<int> Lvl1Rec = {2, 1, 0}; //Scouts, Chasers, Coins
    const std::vector<std::string> Lvl1Dialogue = {"Welcome to the Outer Rim!", "To pass the gate, I need some biological samples.", "Bring me: 2 Scouts and 1 Chaser."};
    //ЛВЛ2
    const int PlayerPos2X = 1;
    const int PlayerPos2Y = 5;
    const std::vector<int> Lvl2Rec = {2, 2, 3};
    const std::vector<std::string> Lvl2Dialogue = {"The Duke is not pleased with your arrival.", "The toll for this bridge is high, traveler.", "Bring me: 2 Scouts, 2 Chaser and 3 Coins"};
    //ЛВЛ3
    const int PlayerPos3X = 3;
    const int PlayerPos3Y = 4;
    const std::vector<std::string> Lvl3Dialogue = {"Wait! The monsters ahead are too strong.", "I need 5 Chaser drops to craft you a shield.", "Don't go further without it!"};
    const std::vector<int> Lvl3Rec = {1, 5, 5};
    //Базовая
    const std::vector<std::string> Base = {"Congratulations! You've reached the end of the demo."};
  }


  namespace Tile {
    const float Size = 128.f;
  }

  namespace Player {
    // Начальные характеристики
    const int StartHP = 100;
    const float Damage = 5.f;

    // Текстуры
    inline const std::string TexIdlePath = "assets/text.png";
    //Все текстурки перемещения пока одинаковые
    inline const std::string TexAttackPath = "assets/attack.png";

    // Физика и управление
    const float SpeedMultiplier = 150.f;     // Базовый множитель для скорости шага
    const float InputLockDivisor = 8.f;      // Доля тайла для разблокировки ввода
    
    // Начальный инвентарь
    const int StartCoins = 0;
    const int StartScoutDrops = 0;
    const int StartChaserDrops = 0;

    // Физика и анимация
    const float MoveSpeed = 2.0f;
    const float AnimationFrameTime = 0.12f;
  }

  namespace BaseStats {
    const int DefaultHP = 50;
    const float DefaultDamage = 5.f;
  }

  namespace Monsters {
    const float AutoScaleFactor = 0.7f;     // Занимает 70% ширины клетки
    const float ArrivalThreshold = 2.0f;    // Дистанция, при которой считается, что монстр "пришел"
    const float CharRandomFactor = 2/3;
  }

  namespace Scout {
    inline const std::string TexturePath = "assets/Scout.png";
    const sf::Color ColorModifier = sf::Color(150, 255, 150);
    const int MaxHP = 30;
    const int Damage = 10;
    const int XpDrop = 50;
    const float MoveSpeed = 150.f;
    
    // Анимация покачивания
    const float AnimSpeedMultiplier = 10.f;
    const float MaxRotationAngle = 5.f;
    
    // Параметры патрулирования
    const int NumDirections = 4;
  }

  namespace Waiter {
    inline const std::string TexturePath = "assets/waiter.png";
    inline const std::string GreetSoundPath = "assets/waiter_voice.mp3";
    const int MaxHP = 30;
    const int Damage = 10;
    const int XpDrop = 50;
    // Анимация дыхания
    const float BreathSpeed = 3.f;
    const float BreathAmplitude = 0.04f;
    
    // Логика
    const int GreetRadius = 1;
  }

  namespace Chaser {
    inline const std::string TexturePath = "assets/chaser.png";
    const int MaxHP = 40;
    const int Damage = 15;
    const int XpDrop = 100;
    const float MoveSpeed = 120.f;
    
    
    const float AnimFrameTime = 0.15f;
    const float Scale = 2.2f;
    const float AgroRange = 3.0f; // Дистанция обнаружения игрока
  }

  namespace Chest {
    inline const std::string TexClosedPath = "assets/chest_closed.png";
    inline const std::string TexOpenPath = "assets/chest_opened.png";
    const int InitHeal = 20;
    const float Scale = 0.15f;
    // Оффсет, чтобы сундук не "тонул" в земле, если спрайт так нарисован
    const float OriginOffset = 128.f; 
  }

  namespace Map {
    inline const std::string FloorTexture = "assets/ground.png";
    inline const std::string WallTexture = "assets/wall.png";

    // Символы в текстовом файле уровня
    namespace Symbols {
      const char Wall    = '#';
      const char Scout   = 'S';
      const char Chaser  = 'H';
      const char Waiter  = 'W';
      const char Chest   = 'C';
      const char NPC     = 'N';
    }
  }

  namespace Dialogue {
    const sf::Color BgColor = sf::Color(30, 30, 30, 220);
    const sf::Color OutlineColor = sf::Color::White;
    const float OutlineThickness = 2.f;

    const float BoxWidth = 700.f;
    const float BoxHeight = 200.f;
    const float BottomOffset = 340.f; // Отступ снизу экрана
    const float Padding = 40.f;      // Внутренний отступ текста от краев

    const unsigned int MainCharSize = 24; //Эти значения не могут быть отрицательными
    const unsigned int ActionCharSize = 20;

    inline const std::string SuccessQuestLine = "Thanks! Move in!";
    inline const std::string ActionGivePrompt = "[E] give tropheys";
    inline const std::string ActionMissingPrompt = "You don't have it";
  }

  namespace Battle {
    // UI
    const sf::Vector2f BoxSize = {800.f, 600.f};
    const sf::Color BgColor = sf::Color(30, 30, 30, 230);
    const sf::Color GridColor = sf::Color(70, 70, 70);
    const float OutlineThickness = 5.f;
    const unsigned int MainTextSize = 40;
    const sf::Vector2f MainTextOffset(20, 20);

    // Сетка
    const float CellSize = 60.f;
    const int GridSize = 5; // Сетка 5x5
    const sf::Vector2f GridVisualSize = {300.f, 300.f}; // 5 * 60

    // Тайминги
    const float ShowPathTime = 1.5f;
    const float ResultWaitTime = 2.0f;
    const float MaxInputTime = 5.0f;
    const float DodgeWindow = 0.5f;
    const float DodgeResultTime = 1.2f;
    const float TimeToDodge = 0.5f;
    
    // Фазы атаки врага (в секундах от начала хода)
    const float EnemyAttackStart = 4.5f;
    const float EnemyAttackEnd = 5.5f;
    const float EnemyInfoEnd = 7.5f;

    // Баланс
    const int BaseHeal = 2;
    const int HealMultiplier = 2; 

    //Линия паттерная боя
    const float InlineThickness = 6.f;
    const sf::Vector2f LineOrigin = {0, 3.f};
    const int FightDifficulty = 6;
    
    
    //Для перевода из радиан в градусы
    const float RadTransFactor = 180.f / 3.14159f;

    //Heal игрока 
    

    // Рандом QTE
    const float QteMinWait = 1.0f;
    const int QteRandomRange = 300; // для rand() % 300 / 100.f

  }

  namespace NPC {
    const sf::Vector2f NpcScale{0.25f, 0.25f};
  }
  
  namespace Cutscene {
    // Настройки текста
    const unsigned int SubtitleSize = 40;
    const float SubtitleOutline = 3.f;
    const sf::Color SubtitleColor = sf::Color::White;
    const sf::Color SubtitleOutlineColor = sf::Color::Black;

    
    // Позиция субтитров (отступ слева и снизу)
    const sf::Vector2f SubtitleOffset = { 100.f, 500.f }; 

    // Математика прогресса
    const float MaxProgress = 1.0f;
    const float DefaultZoom = 1.0f;
  }
  
}
