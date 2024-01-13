#include "Game.h"
#include <iostream>

// To-do list :
// 
// -Multi texture handle for Graphic_Objects
// -Multi variables for VBOs
// -Map handling
// -Player handling
// -New VBOs models
// -Physic simulation

int main()
{
    // Construct game
    Game game(1600, 900);

    // Construct scene
    Scene* scene = game.new_scene("level0");
    game.set_current_scene("level0");

    scene->new_object("sol", "square", 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "textures/unknow.png");

    game.run();

    return 0;
}