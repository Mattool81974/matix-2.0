#include "Game.h"
#include <iostream>

// To-do list :
// -Map handling
// -Player handling
// -New VBOs models
// -Physic simulation

int main()
{
    // Construct game
    Game game(1600, 900);
    game.new_part(1, "cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 3, 1), "../textures/wall.png");
    game.new_part(2, "cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 3, 1), "../textures/pillar.png");

    // Construct scene
    Scene* scene = game.new_scene("level0", "../maps/level0.wad");
    game.set_current_scene("level0");

    game.run();

    return 0;
}