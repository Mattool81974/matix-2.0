#include "../headers/game.h"
#include "../headers/test.h"

Game* game = 0;

void load_cli()
{
    game->set_background_color(glm::vec4(0, 0, 0, 1));

    // Construct the HUD
    HUD_CLI* hud = game->new_hud<HUD_CLI>("cli");
    hud->load_from_file(game->get_assets_directory_path() + "cli/normal_fr.cli");
}

void load_warehouse()
{
    // Construct game
    game->set_background_color(glm::vec4(0.0f, (1.0f / 255.0f) * 204.0f, (1.0f / 255.0f) * 204.0f, 1.0f));
    Camera* camera = game->get_camera();
    game->new_vao(game->get_assets_directory_path() + "vbos/famas.vbo", "famas");
    game->new_vao(game->get_assets_directory_path() + "vbos/shell.vbo", "ammo");
    Part* computer = game->new_part(7, "cube", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), game->get_assets_directory_path() + "textures/computer.png");
    Part* floor_exterior = game->new_part(1, "one_faced_cube", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), game->get_assets_directory_path() + "textures/warehouse/floor_exterior.png");
    Part* floor_interior = game->new_part(2, "one_faced_cube", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), game->get_assets_directory_path() + "textures/warehouse/floor_interior.png");
    Part* door = game->new_part<Door>(3, "cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(0.1, 2, 1), game->get_assets_directory_path() + "textures/warehouse/door.png");
    Part* package = game->new_part(4, "cube", glm::vec3(0, 1.0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), game->get_assets_directory_path() + "textures/warehouse/package.png");
    Part* glass = game->new_part(5, "one_faced_cube", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), game->get_assets_directory_path() + "textures/warehouse/glass.png");
    Part* table = game->new_part(6, "table", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), game->get_assets_directory_path() + "textures/table.png");
    Part* wall_exterior = game->new_part(10, "one_faced_cube", glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), game->get_assets_directory_path() + "textures/warehouse/wall_exterior.png");

    // Configurate parts
    computer->set_description("computer");
    door->set_description("door");
    door->set_resize_texture(false);
    door->set_use_collection(false);
    glass->set_description("glass");
    glass->set_is_transparent(true);
    glass->set_resize_texture(false);
    floor_exterior->set_description("exterior floor");
    floor_interior->set_description("interior floor");
    package->set_description("package");
    table->set_resize_texture(false);
    wall_exterior->set_description("wall");
    wall_exterior->set_scale_level_multiplier(glm::vec3(0, 1, 0));

    // Construct scene
    Scene* scene = game->new_scene("warehouse", game->get_assets_directory_path() + "maps/warehouse.wad", Map_Opening_Mode::Complex);

    // Construct objects for testing
    Player* player = scene->new_object<Player>("player", "player", 0, glm::vec3(13, 3, 53), glm::vec3(0, 0, 0), glm::vec3(1.3, 1.75, 1.3), false, "", false, false, true);
    Famas* famas = scene->new_object<Famas>("famas", "famas", camera, glm::vec3(0, 0, 0), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), true, game->get_assets_directory_path() + "textures/famas.png", false, true, false);

    // Construct the HUD
    HUD* hud = game->new_hud("base");
    HUD_Text* fps = hud->new_hud_object<HUD_Text>("fps", game->get_assets_directory_path() + "fonts/default.png", "default_font");
    HUD_Object* watermark = hud->new_hud_object("watermark", game->get_assets_directory_path() + "textures/watermark.png");

    // Configurate some objects in the scene
    camera->set_parent(player->get_attached_transform());
    camera->set_position(glm::vec3(0, 0.75, 0));
    famas->get_attached_transform()->set_anchored_position(glm::vec3(-0.5, 0.15, 0.3));
    famas->get_attached_transform()->set_parent_rotation_multiplier(glm::vec3(1.0f, -1.0f, 1.0f));
    player->get_attached_physic_object()->set_use_collision(true);
    player->get_attached_physic_object()->set_elasticity(0.0f);
    player->set_map_pos(glm::vec3(2, 1, 2));
    player->set_description("player");

    // Configurate the HUD
    std::string texte_fps = "FPS : 0.";
    fps->set_font_color(glm::vec4(1, 1, 1, 1));
    fps->set_position(glm::vec3(0.65, 0.9, 0));
    fps->set_scale(glm::vec3(0.1, 0.1, 1));
    fps->set_text(texte_fps);
    watermark->set_position(glm::vec3(-0.6, 0.76, 0.9));
    watermark->set_scale(glm::vec3(0.4, 0.24, 1));
}

void level0()
{
    // Construct game
    Camera* camera = game->get_camera();
    game->new_vao("../vbos/famas.vbo", "famas");
    game->new_vao("../vbos/shell.vbo", "ammo");
    game->new_part(1, "one_faced_cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 3, 1), "../textures/wall.png");
    game->new_part(2, "cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 3, 1), "../textures/pillar.png");

    // Construct scene
    Scene* scene = game->new_scene("level0", "../maps/level0.wad");
    game->set_current_scene("level0");

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

    while (game->run())
    {
        game->update_event();

        game->update();
    }
}

void shooting_range()
{
    // Construct game
    Camera* camera = game->get_camera();
    game->new_vao("../vbos/famas.vbo", "famas");
    game->new_vao("../vbos/shell.vbo", "ammo");
    game->new_part(1, "one_faced_cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 3, 1), "../textures/dark_wall.png");
    game->new_part(2, "table", glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/table.png");

    // Construct scene
    Scene* scene = game->new_scene("shooting_range", "../maps/shooting_range.wad");
    game->set_current_scene("shooting_range");

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

    while (game->run())
    {
        game->update_event();

        game->update();
    }
}

int main(int argc, char* argv[])
{
    srand(time(0));

    game = new Game(1600, 900, argv[0], false);
    game->load_from_config_file("../matix_config.cfg");
    game->load_VAOs();

    load_cli();
    load_warehouse();

    game->set_current_hud("base");
    game->set_current_scene("warehouse");

    Camera* camera = game->get_camera();
    HUD_CLI* cli = (HUD_CLI*)game->get_hud("cli");
    HUD_Text* fps = (HUD_Text*)game->get_hud("base")->get_hud_object("fps");
    Player* player = (Player*)game->get_scene("warehouse")->get_object("player");
    Scene* warehouse = game->get_scene("warehouse");
    std::string texte_fps = "FPS : 0.";

    bool current_is_cli = false;

    while (game->run())
    {
        game->update_event();
        game->update();

        if (!current_is_cli && game->get_key_state_frame("e") == Key_State::Pressed)
        {
            One_Raycast rc = warehouse->get_physic_scene()->raycast(camera->get_absolute_position(), game->get_camera()->get_forward());
            if (rc.touched_object != 0)
            {
                if (((Object*)(rc.touched_object))->get_type() == "door")
                {
                    ((Door*)(rc.touched_object))->interact();
                }
                else if (((Object*)(rc.touched_object))->get_description() == "computer")
                {
                    current_is_cli = true;
                    game->set_background_color(glm::vec4(0, 0, 0, 1));
                    game->set_current_hud("cli");
                    warehouse->set_use_graphic(false);
                }
            }
        }

        if (!current_is_cli)
        {
            texte_fps = "FPS : " + std::to_string((int)glm::round(1.0 / game->get_delta_time())) + ".";
            fps->set_text(texte_fps);
        }
        else
        {
            std::string current_command = cli->get_current_command();
            if (current_command == "return")
            {
                current_is_cli = false;
                game->set_background_color(glm::vec4(0.0f, (1.0f / 255.0f) * 204.0f, (1.0f / 255.0f) * 204.0f, 1.0f));
                game->set_current_hud("base");
                cli->set_current_command("");
                warehouse->set_use_graphic(true);
            }
        }

        game->render();
    }

    delete game;

    return 0;
}