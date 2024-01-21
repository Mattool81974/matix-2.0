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
    Camera* camera = game.get_base_struct()->get_camera();
    game.get_advanced_struct()->new_vao("../vbos/famas.vbo", "famas");
    game.get_advanced_struct()->new_vao("../vbos/shell.vbo", "shell");
    game.new_part(1, "cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 3, 1), "../textures/wall.png");
    game.new_part(2, "cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 3, 1), "../textures/pillar.png");

    // Construct scene
    Scene* scene = game.new_scene("level0", "../maps/level0.wad");
    game.set_current_scene("level0");

    // Construct objects for testing
    Object *player = scene->new_object("player", "player", 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "", false, false, true);
    scene->new_object("sol", "square", 0, glm::vec3(12, 0, 12), glm::vec3(90, 0, 0), glm::vec3(25, 25, 1), "../textures/floor.png");
    scene->new_object("locker", "cube", 0, glm::vec3(2, 1, 2), glm::vec3(0, 0, 0), glm::vec3(1, 2, 1), "../textures/locker.png", false);
    scene->new_object("computer", "cube", 0, glm::vec3(2, 0.5, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/computer.png", false);
    scene->new_object("table", "table", 0, glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/table.png", false);
    scene->new_object("chair", "chair", 0, glm::vec3(1, 0.5, 0), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), "../textures/chair.png", false);
    scene->new_object("clock", "circle", 0, glm::vec3(4.49, 2.25, 0), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), "../textures/clock.png", false);
    scene->new_object("ammo1", "cylinder", 0, glm::vec3(0.25, 1.05, 0), glm::vec3(270, 0, 0), glm::vec3(0.05, 0.05, 0.1), "../textures/ammo.png", false);
    scene->new_object("ammo2", "cylinder", 0, glm::vec3(0, 1.05, 0), glm::vec3(270, 0, 0), glm::vec3(0.05, 0.05, 0.1), "../textures/ammo.png", false);
    scene->new_object("ammo3", "cylinder", 0, glm::vec3(0, 1.05, 0.25), glm::vec3(270, 0, 0), glm::vec3(0.05, 0.05, 0.1), "../textures/ammo.png", false);
    Object *famas = scene->new_object("famas", "famas", player->get_attached_transform(), glm::vec3(1, -0.9, 0.35), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), "../textures/famas.png", false);
    scene->new_object("luxary_famas", "famas", 0, glm::vec3(0, 3, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/luxary_famas.png", false);
    scene->new_object("shell", "shell", 0, glm::vec3(1, 3.6725, -0.03), glm::vec3(0, 180, 0), glm::vec3(0.01, 0.01, 0.03), "../textures/shell.png", false);

    // Configurate some objects in the scene
    camera->set_parent(player->get_attached_transform());

    game.run();

    return 0;
}