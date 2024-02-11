#include "../headers/lunar_rover.h"

namespace Lunar_Rover
{
    // Main game variables
    Camera* camera = 0;
    Game* game = 0;
    Scene* lunar_scene = 0;

    // Rover constructor
    Rover::Rover(Advanced_Struct* a_advanced_struct, std::string a_name , std::string a_scene_name , Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Object(a_advanced_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
    {

    }

    // Advanced game variables
    Rover* rover = 0;

    // Load the lunar environment
    void load_lunar_environment()
    {
        // Configure the game
        camera = game->get_camera();

        // Add the needed parts
        Part* floor = game->new_part(1, "one_faced_cube", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), game->get_assets_directory_path() + "textures/lunar_rover/floor.png");

        // Create the lunar scene
        lunar_scene = game->new_scene("moon", game->get_assets_directory_path() + "maps/moon.wad", Map_Opening_Mode::Complex);

        // Create the exterior objects
        rover = lunar_scene->new_object<Rover>("rover", "", 0, glm::vec3(10, 2, 10), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, "", false, false, true);
    }

    // Setup the lunar environment
    void set_lunar_environment()
    {
        // Set some game attributes
        game->set_background_color(glm::vec4(0, 0, 0, 1.0));

        // Set the current scene
        game->set_current_scene("moon");
    }

    // Run the lunar rover game
    void run_lunar_rover(std::string path)
    {
        game = new Game(1600, 900, path, false);
        game->load_from_config_file("../matix_config.cfg");
        game->load_VAOs();

        load_lunar_environment();
        set_lunar_environment();

        while (game->run())
        {
            game->update_event();
            game->update();

            camera->set_position(glm::vec3(0, 3, 0));

            game->render();
        }

        delete game;
        game = 0;
    }
}