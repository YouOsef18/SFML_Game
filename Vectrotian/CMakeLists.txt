cmake_minimum_required(VERSION 3.15)
project(Vectrotian)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем путь к SFML (для Homebrew на Apple Silicon)
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

# В SFML 3 компоненты часто требуют написания с большой буквы в find_package
find_package(SFML 3 COMPONENTS Window Graphics System Audio REQUIRED)

add_executable(SFML_App 
    main.cpp
    game.cpp 
    map.cpp 
    player.cpp 
    monster.cpp 
    chest.cpp 
    battle_manager.cpp 
    battle_renderer.cpp
    scout.cpp
    chaser.cpp 
    waiter.cpp
    dialogue_manager.cpp
    cutscene_manager.cpp

)
target_link_libraries(SFML_App 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio  
)