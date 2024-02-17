#include "../headers/game.h"
#include "../mods/headers/lunar_rover.h"
#include "../mods/headers/player.h"

void cube_3d(std::string exec_path)
{
    Game* game = new Game(1600, 900, exec_path, false);
    game->load_from_config_file("../matix_config.cfg");
    game->load_VAOs();

    Camera* camera = game->get_camera();
    camera->set_looks_forward(false);
    camera->set_position(glm::vec3(2, 1, 2));

    float axis_with = 0.03;
    Scene* scene = game->new_scene("scene");
    Object* cube = scene->new_object("cube", "one_faced_cube", 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), true, game->get_assets_directory_path() + "textures/cool_square.png", true, true, false);
    Object* x = scene->new_object("x", "one_faced_cube", 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(100, axis_with, axis_with), true, game->get_assets_directory_path() + "textures/white.png", true, true, false);
    Object* y = scene->new_object("y", "one_faced_cube", 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(axis_with, 100, axis_with), true, game->get_assets_directory_path() + "textures/white.png", true, true, false);
    Object* z = scene->new_object("z", "one_faced_cube", 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(axis_with, axis_with, 100), true, game->get_assets_directory_path() + "textures/white.png", true, true, false);
    cube->get_attached_graphic_object()->set_is_transparent(true);

    game->set_current_scene("scene");

    game->set_background_color(glm::vec4((1.0 / 255.0) * 30.0, (1.0 / 255.0) * 30.0, (1.0/255.0)*30.0, 1.0));

    while (game->run())
    {
        game->update_event();
        game->update();

        game->render();
    }

    delete game;
    game = 0;
}

int main(int argc, char* argv[])
{
    srand(time(0));

    // cube_3d(argv[0]);

    Lunar_Rover::run_lunar_rover(argv[0]);

    return 0;
}