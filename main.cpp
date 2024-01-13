#include "Game.h"

int main()
{
    // Construct game
    Game game(1600, 900);
    Base_Struct* base_struct = game.get_base_struct();

    // Construct scene
    Scene* scene = game.new_scene("level0");
    game.set_current_scene("level0");

    VAO vao = VAO("shaders/default");

    game.run();

    return 0;
}