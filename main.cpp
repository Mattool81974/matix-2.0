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
    game.new_part(1, "../textures/wall.png");

    // Construct scene
    Scene* scene = game.new_scene("level0");
    game.set_current_scene("level0");

    scene->new_object("sol", "cube", 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/unknow_cube.png");
    scene->new_object("sol1", "cube", 0, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/unknow_cube.png");
    scene->new_object("sol2", "cube", 0, glm::vec3(2, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/unknow_cube.png");
    scene->new_object("sol3", "cube", 0, glm::vec3(3, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/wall.png");

    game.run();

    return 0;
}