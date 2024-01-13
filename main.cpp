#include "Game.h"
#include <iostream>

int main()
{
    // Construct game
    Game game(1600, 900);

    // Construct scene
    Scene* scene = game.new_scene("level0");
    game.set_current_scene("level0");

    scene->new_object("sol");

    game.run();

    return 0;
}