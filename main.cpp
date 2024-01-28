#include "Game.h"
#include <iostream>
#include "test.h"

void warehouse()
{
    // Construct game
    Game game(1600, 900);
    Camera* camera = game.get_camera();
    game.new_vao("../vbos/famas.vbo", "famas");
    game.new_vao("../vbos/shell.vbo", "ammo");
    Part* floor_exterior = game.new_part(1, "one_faced_cube", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/warehouse/floor_exterior.png");
    Part* floor_interior = game.new_part(2, "one_faced_cube", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/warehouse/floor_interior.png");
    Part* door = game.new_part<Door>(3, "cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(0.1, 2, 1), "../textures/warehouse/door.png");
    Part* wall_exterior = game.new_part(10, "one_faced_cube", glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/warehouse/wall_exterior.png");

    // Configurate parts
    door->set_resize_texture(false);
    door->set_use_collection(false);
    wall_exterior->set_scale_level_multiplier(glm::vec3(0, 1, 0));

    // Construct scene
    Scene* scene = game.new_scene("warehouse", "../maps/warehouse.wad", Map_Opening_Mode::Collections);
    game.set_current_scene("warehouse");

    // Construct objects for testing
    Object* player = scene->new_object("player", "player", 0, glm::vec3(2, 1, 2), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, "", false, false, true);
    Famas* famas = scene->new_object<Famas>("famas", "famas", camera, glm::vec3(0, 0, 0), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), true, "../textures/famas.png", false, true, false);
    
    // Configurate some objects in the scene
    camera->set_parent(player->get_attached_transform());
    camera->set_position(glm::vec3(0, 0.75, 0));
    famas->get_attached_transform()->set_anchored_position(glm::vec3(-0.3, -0.15f, -0.45));
    famas->get_attached_transform()->set_parent_rotation_multiplier(glm::vec3(1.0f, -1.0f, 1.0f));
    player->get_attached_physic_object()->get_collision()->set_height(2);
    player->get_attached_physic_object()->get_collision()->set_width(0.65);
    player->get_attached_physic_object()->set_use_collision(true);

    while (game.run())
    {
        game.update_event();

        game.update();
    }
}

void level0()
{
    // Construct game
    Game game(1600, 900);
    Camera* camera = game.get_camera();
    game.new_vao("../vbos/famas.vbo", "famas");
    game.new_vao("../vbos/shell.vbo", "ammo");
    game.new_part(1, "one_faced_cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 3, 1), "../textures/wall.png");
    game.new_part(2, "cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 3, 1), "../textures/pillar.png");

    // Construct scene
    Scene* scene = game.new_scene("level0", "../maps/level0.wad");
    game.set_current_scene("level0");

    // Construct objects for testing
    Object* chair = scene->new_object("chair", "chair", 0, glm::vec3(9, 0.5, 14), glm::vec3(0, 90, 0), glm::vec3(1, 1, 1), true, "../textures/chair.png", false, true, false);
    Object* clock = scene->new_object("clock", "circle", 0, glm::vec3(10.49, 2, 11.5), glm::vec3(0, 90, 0), glm::vec3(1, 1, 1), true, "../textures/clock.png", false, true, false);
    Object* computer = scene->new_object("computer", "cube", 0, glm::vec3(10, 1.5, 14), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), true, "../textures/computer.png", false, true, false);
    Famas* famas = scene->new_object<Famas>("famas", "famas", camera, glm::vec3(0, 0, 0), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), true, "../textures/famas.png", false, true, false);
    Object* player = scene->new_object("player", "player", 0, glm::vec3(2, 1, 2), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, "", false, false, true);
    Object* floor = scene->new_object("floor", "square", 0, glm::vec3(20, 0, 20), glm::vec3(90, 0, 0), glm::vec3(50, 50, 1), true, "../textures/floor.png");
    Object* locker = scene->new_object("locker", "cube", 0, glm::vec3(10, 1, 10), glm::vec3(0, 0, 0), glm::vec3(1, 2, 1), true, "../textures/locker.png", false, true, false);
    Object* table = scene->new_object("table", "table", 0, glm::vec3(10, 0.5, 14), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), true, "../textures/table.png", false, true, false);
    
    // Configurate some objects in the scene
    camera->set_parent(player->get_attached_transform());
    camera->set_position(glm::vec3(0, 0.75, 0));
    famas->get_attached_transform()->set_anchored_position(glm::vec3(-0.3, -0.15f, -0.45));
    famas->get_attached_transform()->set_parent_rotation_multiplier(glm::vec3(1.0f, -1.0f, 1.0f));
    player->get_attached_physic_object()->get_collision()->set_height(2);
    player->get_attached_physic_object()->get_collision()->set_width(0.65);
    player->get_attached_physic_object()->set_use_collision(true);

    while (game.run())
    {
        game.update_event();

        game.update();
    }
}

void shooting_range()
{
    // Construct game
    Game game(1600, 900);
    Camera* camera = game.get_camera();
    game.new_vao("../vbos/famas.vbo", "famas");
    game.new_vao("../vbos/shell.vbo", "ammo");
    game.new_part(1, "one_faced_cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 3, 1), "../textures/dark_wall.png");
    game.new_part(2, "table", glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/table.png");

    // Construct scene
    Scene* scene = game.new_scene("shooting_range", "../maps/shooting_range.wad");
    game.set_current_scene("shooting_range");

    // Construct objects for testing
    Object* player = scene->new_object("player", "player", 0, glm::vec3(2, 1, 2), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, "", false, false, true);
    scene->new_object("sol", "square", 0, glm::vec3(6, 0, 26), glm::vec3(90, 0, 0), glm::vec3(12, 52, 1), true, "../textures/floor.png");
    scene->new_object("plafond", "square", 0, glm::vec3(6, 3, 26), glm::vec3(90, 0, 0), glm::vec3(12, 52, 1), true, "../textures/floor.png");
    Famas* famas = scene->new_object<Famas>("famas", "famas", camera, glm::vec3(0, 0, 0), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), true, "../textures/famas.png", false, true, false);
    Target* target1 = scene->new_object<Target>("target1", "cube", 0, glm::vec3(0, 0, 0), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), true, "../textures/target/target1.png", false, true, true);

    // Configurate some objects in the scene
    camera->set_parent(player->get_attached_transform());
    camera->set_position(glm::vec3(0, 0.75, 0));
    famas->get_attached_transform()->set_anchored_position(glm::vec3(-0.3, -0.15f, -0.45));
    famas->get_attached_transform()->set_parent_rotation_multiplier(glm::vec3(1.0f, -1.0f, 1.0f));
    player->get_attached_physic_object()->get_collision()->set_height(2);
    player->get_attached_physic_object()->get_collision()->set_width(0.65);
    player->get_attached_physic_object()->set_use_collision(true);
    player->get_attached_transform()->set_position_move_multipler(glm::vec3(1, 0, 1));
    target1->new_texture("../textures/target/target2.png");
    target1->new_texture("../textures/target/target3.png");
    target1->new_texture("../textures/target/target4.png");
    target1->new_texture("../textures/target/target5.png");
    target1->set_max_pos(glm::vec2(11, 51));
    target1->set_min_pos(glm::vec2(1, 20));
    target1->set_bottom_y(0.5);
    target1->set_top_y(2.5);

    while (game.run())
    {
        game.update_event();

        game.update();
    }
}

int main()
{
    srand(time(0));

    warehouse();

    return 0;
}