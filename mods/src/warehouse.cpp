#include "../headers/warehouse.h"

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
    Player* player = scene->new_object<Player>("player", "player", 0, glm::vec3(15, 3, 20), glm::vec3(0, 0, 0), glm::vec3(1.3, 1.75, 1.3), false, "", false, false, true);
    // Famas* famas = scene->new_object<Famas>("famas", "famas", camera, glm::vec3(0, 0, 0), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), true, game->get_assets_directory_path() + "textures/famas.png", false, true, false);

    // Construct the HUD
    HUD* hud = game->new_hud("base");
    HUD_Object* cross = hud->new_hud_object("cross", game->get_assets_directory_path() + "textures/cross.png");
    HUD_Text* fps = hud->new_hud_object<HUD_Text>("fps", game->get_assets_directory_path() + "fonts/default.png", "default_font");
    HUD_Object* watermark = hud->new_hud_object("watermark", game->get_assets_directory_path() + "textures/watermark.png");

    // Configurate some objects in the scene
    float cross_width = 0.01;
    camera->set_parent(player->get_attached_transform());
    camera->set_position(glm::vec3(0, 0.75, 0));
    cross->set_position(glm::vec3(0, 0, 0));
    cross->set_scale(glm::vec3(cross_width, cross_width * (16.0 / 9.0), 1));
    // famas->get_attached_transform()->set_anchored_position(glm::vec3(-0.5, 0.15, 0.3));
    // famas->get_attached_transform()->set_parent_rotation_multiplier(glm::vec3(1.0f, -1.0f, 1.0f));
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

    // Create robot
    Robot* robot1 = scene->new_object<Robot>("robot1", "", 0, glm::vec3(20, 1, 20), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, "", false, false, true);
    robot1->get_attached_physic_object()->get_collision()->set_length(1);
    robot1->get_attached_physic_object()->get_collision()->set_width(1);
    robot1->set_screen_texture(game->get_assets_directory_path() + "textures/warehouse/robot/screen.png");
    robot1->create();
}

// Run the warehouse program
void run_warehouse(std::string path)
{
    game = new Game(1600, 900, path, false);
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

    Robot* robot1 = (Robot*)warehouse->get_object("robot1");

    bool current_is_cli = false;

    while (game->run())
    {
        game->update_event();
        game->update();

        if (game->get_key_state("left arrow") == Key_State::Pressed)
        {
            robot1->turn(1);
        }

        if (game->get_key_state("right arrow") == Key_State::Pressed)
        {
            robot1->turn(-1);
        }

        if (game->get_key_state("up arrow") == Key_State::Pressed)
        {
            robot1->forward(1);
        }

        if (game->get_key_state("down arrow") == Key_State::Pressed)
        {
            robot1->forward(-1);
        }

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
    game = 0;
}

// Entity constructor
Entity::Entity(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Object(a_advanced_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
{

}

// Clone the entity
void* Entity::clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic)
{
    return new Entity(a_game_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic);
}

// Entity destructor
Entity::~Entity()
{

}

// Player constructor
Player::Player(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Entity(a_advanced_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
{
    game = (Game*)get_game_struct();
}

// Player the entity
void* Player::clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic)
{
    return new Player(a_game_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic);
}

// Late update the player
void Player::late_update()
{
    update_move();
}

// Update the moving of the player
void Player::update_move()
{
    float delta_time = get_game_struct()->get_delta_time();
    float speed = get_speed();
    speed *= delta_time;

    // Rotate and move camera
    float jump_force = 4;
    float rotate_speed = 45;
    float sensitivity = get_game_struct()->get_camera()->get_sensitivity();
    get_attached_transform()->rotate(glm::vec3(0.0, sensitivity * delta_time * get_game_struct()->get_mouse_move_x(), 0.0));
    get_game_struct()->get_camera()->rotate(glm::vec3(-sensitivity * get_game_struct()->get_delta_time() * get_game_struct()->get_mouse_move_y(), 0.0, 0.0));

    // Move the player
    if (get_game_struct()->get_key_state("z") == 1)
        get_attached_transform()->move(glm::vec3(speed) * get_attached_transform()->get_forward());
    if (get_game_struct()->get_key_state("s") == 1)
        get_attached_transform()->move(glm::vec3(speed) * -get_attached_transform()->get_forward());
    if (get_game_struct()->get_key_state("q") == 1)
        get_attached_transform()->move(glm::vec3(speed) * get_attached_transform()->get_right());
    if (get_game_struct()->get_key_state("d") == 1)
        get_attached_transform()->move(glm::vec3(speed) * -get_attached_transform()->get_right());
    if (get_game_struct()->get_key_state("space") == 1)
    {
        glm::vec3 force = glm::vec3(jump_force) * get_attached_transform()->get_up();
        if (use_physic() && get_attached_physic_object()->is_standing()) get_attached_physic_object()->apply_force(force);
    }
    if (get_game_struct()->get_key_state("left shift") == 1)
        get_attached_transform()->move(glm::vec3(speed) * -get_attached_transform()->get_up());
}

// Player destructor
Player::~Player()
{

}

// Ammo constructor
Ammo::Ammo(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Object(a_advanced_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
{
    game = (Game*)get_game_struct();
    creation_time = glfwGetTime();
}

// Clone the ammo
void* Ammo::clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic)
{
    return new Ammo(a_game_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic);
}

// Late update the ammo
void Ammo::last_update()
{
    if (get_collisions_result()->size() > 0 && !bounce)
    {
        Scene* scene = (*game->get_scenes())[get_scene_name()];
        scene->destroy(get_name());
    }
    Object::last_update();
}

// Update the ammo
void Ammo::update()
{
    if (glfwGetTime() - creation_time > deadline)
    {
        Scene* scene = (*game->get_scenes())[get_scene_name()];
        scene->destroy(get_name());
    }
}

// Ammo destructor
Ammo::~Ammo()
{
    Object::~Object();
}

// Famas constructor
Famas::Famas(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Object(a_advanced_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
{
    game = (Game*)get_game_struct();
    get_attached_transform()->start_animation();
}

// Clone the famas
void* Famas::clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic)
{
    return new Famas(a_game_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic);
}

// Shoot with the famas
void Famas::shoot()
{
    Scene* scene = (*game->get_scenes())[get_scene_name()];

    // Calculate ammo transformation
    glm::vec3 normalized_forward = glm::normalize(get_attached_transform()->get_forward());
    glm::vec3 forward = glm::cross(normalized_forward, glm::vec3(0, -1, 0));
    forward = glm::vec3(-forward[0], normalized_forward[1], forward[2]);

    float x_offset = 0.75;
    float y_offset = 0.8;
    float z_offset = 0.75;
    glm::vec3 position = get_attached_transform()->get_absolute_position() + glm::vec3(0, 0.0, 0.0) + glm::vec3(x_offset, y_offset, z_offset) * forward;
    glm::vec3 rotation = get_attached_transform()->get_rotation() * glm::vec3(-1.0, 1.0, 1.0) + glm::vec3(0, 180, 0);
    glm::vec3 scale = get_ammo_scale();

    // Create the ammo
    float ammo_speed = get_ammo_speed();
    Ammo* ammo = scene->new_object<Ammo>("ammo-" + std::to_string(ammo_shooted), "ammo", 0, position, rotation, scale, false, game->get_assets_directory_path() + "textures/shell.png", false);
    ammo->get_tags()->push_back("ammo");
    ammo->get_attached_physic_object()->get_collision()->set_height(0.1);
    ammo->get_attached_physic_object()->get_collision()->set_width(0.1);
    ammo->get_attached_physic_object()->set_velocity(glm::vec3(ammo_speed, ammo_speed, ammo_speed) * forward);

    // Apply the step back
    glm::vec3 final_position = get_step_back_force();
    forward = glm::normalize(get_attached_transform()->get_forward() * glm::vec3(1, 0, 1));
    float rand_x = 0;
    float rand_y = 0;
    if ((int)(final_position[0] * 2000) != 0) { rand_x = ((glm::abs(final_position[0]) * 1000) - (rand() % (int)(glm::abs(final_position[0]) * 2000))) / 1000; }
    if ((int)(final_position[1] * 2000) != 0) { rand_y = ((glm::abs(final_position[1]) * 1000) - (rand() % (int)(glm::abs(final_position[1]) * 2000))) / 1000; }
    final_position[0] = 0;
    final_position[2] = rand_x;
    final_position = rotate_vector(final_position, -get_attached_transform()->get_rotation(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    final_position[1] = rand_y;
    get_attached_transform()->reset_animation();
    get_attached_transform()->add_position_animation(get_step_back_duration(), get_attached_transform()->get_position_animation() - final_position, get_attached_transform()->get_position());
    get_attached_transform()->add_position_animation(get_step_back_duration(), get_attached_transform()->get_position(), get_attached_transform()->get_position_animation() - final_position);

    // Update shoot datas
    ammo_shooted += 1;
    last_ammo_shooted = glfwGetTime();
}

// Update the famas
void Famas::update()
{
    if (get_game_struct()->get_left_mouse_button_state()) // If the left button is pressed
    {
        if (glfwGetTime() - last_ammo_shooted > 1.0f / (float)ammo_by_second)
        {
            shoot();
        }
    }

    if (get_game_struct()->get_right_mouse_button_state() == 0) // If the right button is pressed
    {
        float delta_time = get_game_struct()->get_delta_time() * (1.0f / get_zoom_duration());
        if (zoom_state - delta_time > 0)
        {
            zoom_state -= delta_time;
        }
        else
        {
            zoom_state = 0;
        }
    }
    else
    {
        float delta_time = get_game_struct()->get_delta_time() * (1.0f / get_zoom_duration());
        if (zoom_state + delta_time < 1)
        {
            zoom_state += delta_time;
        }
        else
        {
            zoom_state = 1;
        }
    }
    zoom();
}

// Apply a zoom to the famas
void Famas::zoom()
{
    get_game_struct()->get_camera()->set_fov(get_zoom_normal_fov() - (get_zoom_normal_fov() - get_zoom_fov()) * zoom_state);
    if (zoom_state == 1)
    {
        get_attached_transform()->set_anchored_position(glm::vec3(-0.5, 0.15, 0.0));
    }
    else
    {
        get_attached_transform()->set_anchored_position(glm::vec3(-0.5, 0.15, 0.3));
    }
}

// Famas destructor
Famas::~Famas()
{
    Object::~Object();
}

// Target constructor
Target::Target(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Object(a_advanced_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
{
    get_attached_transform()->set_position(glm::vec3(-1, -1, -1));
    get_textures()->push_back(get_attached_graphic_object()->get_texture()->get_texture_path());
}

// Clone the target
void* Target::clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic)
{
    return new Target(a_game_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic);
}

// Update lately the target
void Target::last_update()
{
    for (int i = 0; i < get_collisions_result()->size(); i++)
    {
        Object* object1 = (Object*)get_collisions_result()->at(i)->object1;
        Object* object2 = (Object*)get_collisions_result()->at(i)->object2;

        Object* collider = object1;
        if (object1 == this)
        {
            collider = object2;
        }

        if (collider->contains_tag("ammo")) // If the target touch an ammo
        {
            undeploy();
        }
    }
    Object::last_update();
}

// Add a texture to the target
void Target::new_texture(std::string texture)
{
    get_textures()->push_back(texture);
    get_game_struct()->get_texture(texture);
}

// Place randomly the target
void Target::place_randomly()
{
    float multiplier = 1.5;
    int rand_texture = rand() % get_textures()->size();
    int rand_x = get_min_pos()[0] + (rand() % (int)(get_max_pos()[0] - get_min_pos()[0]));
    float rand_y = rand() % 2;
    int rand_z = get_min_pos()[1] + (rand() % (int)(get_max_pos()[1] - get_min_pos()[1]));

    if (rand_y == 0)
    {
        multiplier = -1.5;
        rand_y = get_bottom_y();
    }
    else {
        rand_y = get_top_y();
    }

    deployed = true;
    set_map_pos(glm::vec3(rand_x, rand_y, rand_z));
    get_attached_graphic_object()->set_texture(get_game_struct()->get_texture((*get_textures())[rand_texture]));
    get_attached_transform()->set_position(glm::vec3(rand_x, rand_y + multiplier, rand_z));

    // Play the animation
    get_attached_transform()->reset_animation();
    get_attached_transform()->start_animation();
    get_attached_transform()->add_position_animation(deployement_time, glm::vec3(0, 0, 0), glm::vec3(0, -multiplier, 0));
}

// Undeploy the target
void Target::undeploy()
{
    if (is_deployed())
    {
        deployed = false;
        undeployed_time = glfwGetTime();

        // Play the 
        get_attached_transform()->reset_animation(false);
        get_attached_transform()->start_animation();
        get_attached_transform()->add_position_animation(deployement_time, get_attached_transform()->get_position_animation(), glm::vec3(0, 0, 0));
    }
}

// Update the target
void Target::update()
{
    if (!is_deployed() && glfwGetTime() - undeployed_time >= deployement_space)
    {
        place_randomly();
    }
}

// Target destructor
Target::~Target()
{
    Object::~Object();
}

// Door constructor
Door::Door(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Object(a_advanced_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
{
    game = (Game*)get_game_struct();
    reset_map_pos = false;
    set_description("2");
    type = "door";
}

// Function called after the loading of the scene
void Door::after_loading()
{
    get_attached_physic_object()->get_collision()->set_length(1); // Set the collisions values
    get_attached_physic_object()->get_collision()->set_width(1);

    if (!is_linked())
    {
        glm::vec3 position = get_map_pos();
        glm::vec3 other_position = position;
        std::vector<std::vector<std::vector<Object*>>>& map = (*game->get_scene(get_scene_name())->get_objects_map());

        if (position[0] + 1 < map.size() && map[position[0] + 1][position[1]][position[2]] != 0 && map[position[0] + 1][position[1]][position[2]]->get_type() == get_type())
        {
            other_door = (Door*)map[position[0] + 1][position[1]][position[2]];
            other_position = position + glm::vec3(1, 0, 0);
        }
        else if (position[0] - 1 >= 0 && map[position[0] - 1][position[1]][position[2]] != 0 && map[position[0] - 1][position[1]][position[2]]->get_type() == get_type())
        {
            other_door = (Door*)map[position[0] - 1][position[1]][position[2]];
            other_position = position - glm::vec3(1, 0, 0);
        }
        else if (position[2] + 1 < map[position[0]][position[1]].size() && map[position[0]][position[1]][position[2] + 1] != 0 && map[position[0]][position[1]][position[2] + 1]->get_type() == get_type())
        {
            other_door = (Door*)map[position[0]][position[1]][position[2] + 1];
            other_position = position + glm::vec3(0, 0, 1);
        }
        else if (position[2] - 1 >= 0 && map[position[0]][position[1]][position[2] - 1] != 0 && map[position[0]][position[1]][position[2] - 1]->get_type() == get_type())
        {
            other_door = (Door*)map[position[0]][position[1]][position[2] - 1];
            other_position = position - glm::vec3(0, 0, 1);
        }

        if (other_door != 0)
        {
            other_door->other_door = this;

            set_map_pos(get_attached_transform()->get_absolute_position());
            other_door->set_map_pos(other_door->get_attached_transform()->get_absolute_position());

            get_attached_transform()->start_animation();
            other_door->get_attached_transform()->start_animation();

            if (position[2] == other_position[2])
            {
                glm::vec3 anchor = glm::vec3(-0.5, 0, 0);
                if (position[0] < other_position[0])
                {
                    anchor = -anchor;
                }

                get_attached_transform()->set_position(get_attached_transform()->get_position() + glm::vec3(0, 0, wall_offset) - anchor);
                other_door->get_attached_transform()->set_position(other_door->get_attached_transform()->get_position() + glm::vec3(0, 0, wall_offset) + anchor);

                get_attached_transform()->rotate(glm::vec3(0, 90, 0), false);
                other_door->get_attached_transform()->rotate(glm::vec3(0, 90, 0), false);

                if (position[0] > other_position[0])
                {
                    other_door->get_attached_transform()->rotate(glm::vec3(0, 180, 0), false);
                    first_door = false;
                }
                else
                {
                    get_attached_transform()->rotate(glm::vec3(0, 180, 0), false);
                    other_door->first_door = false;
                }

                get_attached_transform()->set_anchored_position(anchor); // Set the anchor of each door
                other_door->get_attached_transform()->set_anchored_position(-anchor);
            }
            else
            {
                glm::vec3 anchor = glm::vec3(0, 0, -0.5);
                if (position[2] < other_position[2])
                {
                    anchor = -anchor;
                }

                get_attached_transform()->set_position(get_attached_transform()->get_position() + glm::vec3(wall_offset, 0, 0) - anchor);
                other_door->get_attached_transform()->set_position(other_door->get_attached_transform()->get_position() + glm::vec3(wall_offset, 0, 0) + anchor);

                if (position[2] > other_position[2])
                {
                    other_door->get_attached_transform()->rotate(glm::vec3(0, 180, 0), false);
                    first_door = false;
                }
                else
                {
                    get_attached_transform()->rotate(glm::vec3(0, 180, 0), false);
                    other_door->first_door = false;
                }

                anchor = glm::vec3(-0.5, 0, 0);
                if (position[2] < other_position[2])
                {
                    anchor = -anchor;
                }

                get_attached_transform()->set_anchored_position(anchor); // Set the anchor of each door
                other_door->get_attached_transform()->set_anchored_position(-anchor);

                horizontal = false;
                other_door->horizontal = false;
            }

            linked = true; // Set the link to true
            other_door->linked = true;

            close();
            other_door->close();
        }
    }
}

// Clone the door
void* Door::clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic)
{
    return new Door(a_game_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic);
}

// Close the door
void Door::close()
{
    if (is_opened())
    {
        glm::vec3 base_position = glm::vec3(0, get_open_y_rotation(), 0);
        float duration = get_animation_time();
        if (get_attached_transform()->get_current_animation() != 0) // Calculate the duration of the previous animations
        {
            base_position = get_attached_transform()->get_current_animation_rotation();
            duration = get_attached_transform()->get_current_animation()->state;
        }
        get_attached_transform()->reset_animation();

        get_attached_transform()->add_rotation_animation(duration, base_position, glm::vec3(0, get_close_y_rotation(), 0));
        get_attached_physic_object()->set_use_collision(true);

        opened = false;
    }
}

// Interact with the door
void Door::interact()
{
    if (is_opened())
    {
        close();
        if (other_door != 0) other_door->close();
    }
    else
    {
        open();
        if (other_door != 0) other_door->open();
    }
}

// Open the door
void Door::open()
{
    if (!is_opened())
    {
        glm::vec3 base_position = glm::vec3(0, get_close_y_rotation(), 0);
        float duration = get_animation_time();
        if (get_attached_transform()->get_current_animation() != 0) // Calculate the duration of the previous animations
        {
            base_position = get_attached_transform()->get_current_animation_rotation();
            duration = get_attached_transform()->get_current_animation()->state;
        }
        get_attached_transform()->reset_animation();

        get_attached_transform()->add_rotation_animation(duration, base_position, glm::vec3(0, get_open_y_rotation(), 0));
        get_attached_physic_object()->set_use_collision(false);

        opened = true;
    }
}

// Update the door
void Door::update()
{
    Object::update();
}

// Door destructor
Door::~Door()
{
    Object::~Object();
}

// HUD_CLI constructor
HUD_CLI::HUD_CLI(Game* a_advanced_struct, std::string a_name) : HUD(a_advanced_struct, a_name)
{
    game = (Game*)a_advanced_struct;
}

// If the CLI contains a command
bool HUD_CLI::contains_command(std::string command_name)
{
    for (std::map<std::string, CLI_Command>::iterator it = get_commands_name()->begin(); it != get_commands_name()->end(); it++)
    {
        if (it->first == command_name) return true;
    }
    return false;
}

// Execute a command in the CLI
void HUD_CLI::execute_command(std::string command_name)
{
    if (command_name == "") return;

    std::vector<std::string> cutted_command = cut_string(command_name, " "); // Get all the part of the command

    if (!contains_command(cutted_command[0]))
    {
        new_line("Matix CLI", get_unknow_command_message(command_name));
        return;
    }

    CLI_Command command = commands_name[cutted_command[0]];
    if (command == CLI_Command::Quit) // If the quit command is entered
    {
        game->set_is_running(false);
    }
    else if (command == CLI_Command::CWD) // If the CWD command is entered
    {
        new_line("Matix CLI", response["cwd"] + " : " + get_current_working_directory());
    }
    else if (command == CLI_Command::Clear_CLI) // If the clear command is entered
    {
        text_hud.clear();
        user_text.clear();
        unload();
    }
    else if (command == CLI_Command::Datas) // If the info command is entered
    {
        std::string data = response["get_datas"] + " :\n";
        for (std::map<std::string, std::string>::iterator it = datas.begin(); it != datas.end(); it++) // Get each data
        {
            data += "- " + it->first + " : " + it->second + "\n";
        }

        new_line("Matix CLI", data.substr(0, data.size() - 1));
    }
    else if (command == CLI_Command::Get) // Change the part
    {
        if (cutted_command.size() > 1 && cutted_command[1] != "")
        {
            std::string p = cutted_command[1];

            new_line("Matix CLI", variables[p]);
        }
        else
        {
            new_line("Matix CLI", response["get_command_help"]);
        }
    }
    else if (command == CLI_Command::Help)
    {
        std::string final_help = "";
        std::vector<std::string> help = cut_string(response["help"], "\n");

        final_help += help[0]; // Construct the help text
        for (int i = 1; i < help.size(); i++)
        {
            final_help += "\n" + help[i] + " : " + response[help[i] + "_command_help"];
        }

        new_line("Matix CLI", final_help);
    }
}

// Load the CLI after being selected as the new current HUD
void HUD_CLI::load()
{
    set_current_working_directory(game->get_exec_directory());
}

// Load the CLI from the data
void HUD_CLI::load(std::string data)
{
    std::vector<std::string> cutted_datas = cut_string(data, "\n"); // Get every field

    for (int i = 0; i < cutted_datas.size(); i++)
    {
        std::vector<std::string> cutted = cut_string(cutted_datas[i], "^");
        std::vector<std::string> r = cut_string(cutted[2], "&");
        std::string final_variable = "";
        for (int i = 0; i < r.size(); i++) final_variable += r[i] + "\n";
        final_variable = final_variable.substr(0, final_variable.size() - 1);

        if (cutted[0] == "r")
        {
            response[cutted[1]] = final_variable;
        }
        else if (cutted[0] == "d")
        {
            datas[cutted[1]] = final_variable;
            if (cutted[1] == "font_size")
            {
                font_size = string_to_float(final_variable);
            }
        }
        else if (cutted[0] == "v")
        {
            variables[cutted[1]] = final_variable;
        }
    }

    start();
}

// Load the CLi from a file
void HUD_CLI::load_from_file(std::string path)
{
    std::string content = read_file(path);
    if (!content.empty())
    {
        load(content);
    }
}

// Declares a error in the CLI
void HUD_CLI::new_error(std::string error)
{
    new_line("Matix CLI error handler", error);
}

// Add a line to the CLI with a defined text and user
void HUD_CLI::new_line(std::string actual_user, std::string line_text)
{
    // Create each part
    HUD_Text* user = new_hud_object<HUD_Text>("user" + std::to_string(user_text.size()), game->get_assets_directory_path() + "fonts/default.png", "default_font");
    HUD_Text* text = new_hud_object<HUD_Text>("text" + std::to_string(text_hud.size()), game->get_assets_directory_path() + "fonts/default.png", "default_font");

    // Configurate the user HUD
    user->set_background_color(glm::vec4(0, 0, 0, 1));
    user->set_font_color(glm::vec4(1, 1, 1, 1));
    user->set_font_size(get_font_size());
    user->set_position(glm::vec3(-0.975, next_y_position(), 0));
    user->set_scale(glm::vec3(1, 1, 1));
    user->set_text(actual_user + " : ");

    // Configurate the text HUD
    std::string input_text = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789:/._";
    text->set_background_color(glm::vec4(0, 0, 0, 1));
    text->set_cursor_character("|");
    text->set_focused(true);
    text->set_font_color(glm::vec4(1, 1, 1, 1));
    text->set_font_size(get_font_size());
    text->set_input(true);
    text->set_input_text(input_text);
    text->set_position(glm::vec3(-0.975 + get_font_size() * ((user->get_text().size() / 2.0) * (9.0 / 11.0)), next_y_position(), 0));
    text->set_scale(glm::vec3(1, 1, 1));
    text->set_text(line_text);
    text->set_use_cursor(true);

    // Update the input text
    if (text_hud.size() > 0)
    {
        text_hud[text_hud.size() - 1]->set_use_cursor(false);
        text_hud[text_hud.size() - 1]->set_focused(false);
    }

    user_text.push_back(user);
    text_hud.push_back(text);
}

// Add a line to the CLI
void HUD_CLI::new_line()
{
    new_line(get_current_user(), "");
}

// Return the y position of the next line
float HUD_CLI::next_y_position()
{
    float line_multiplicator = 1.1f;
    float multiplicator = 1.25;
    float total = 1.0f - get_font_size() * line_multiplicator;
    for (int i = 0; i < user_text.size(); i++) // Calculate the y pos
    {
        unsigned int line_size = cut_string(text_hud[i]->get_text(), "\n").size();
        float n = 0;
        for (int j = 0; j < line_size; j++) n += text_hud[i]->get_font_size() * multiplicator;
        if (line_size > 0) n *= line_multiplicator;
        total -= n;
    }

    return total;
}

// Start the CLI
void HUD_CLI::start()
{
    new_line("Matix CLI", response["welcome"]);

    // Load base commands
    // Quit
    commands_name["arrete"] = CLI_Command::Quit;
    commands_name["quit"] = CLI_Command::Quit;
    commands_name["quitter"] = CLI_Command::Quit;
    commands_name["quitte"] = CLI_Command::Quit;
    commands_name["stop"] = CLI_Command::Quit;
    // Clear
    commands_name["clear"] = CLI_Command::Clear_CLI;
    commands_name["empty"] = CLI_Command::Clear_CLI;
    commands_name["vide"] = CLI_Command::Clear_CLI;
    commands_name["nettoie"] = CLI_Command::Clear_CLI;
    // CWD
    commands_name["cwd"] = CLI_Command::CWD;
    // Datas
    commands_name["data"] = CLI_Command::Datas;
    commands_name["datas"] = CLI_Command::Datas;
    commands_name["donnee"] = CLI_Command::Datas;
    commands_name["donnees"] = CLI_Command::Datas;
    // Part
    commands_name["get"] = CLI_Command::Get;
    commands_name["avoir"] = CLI_Command::Get;
    commands_name["voir"] = CLI_Command::Get;
    // Help
    commands_name["help"] = CLI_Command::Help;
    commands_name["aide"] = CLI_Command::Help;
    commands_name["sos"] = CLI_Command::Help;
}

// Update the CLI
void HUD_CLI::update()
{
    current_command = "";
    update_object();

    if (get_advanced_struct()->get_key_state_frame("enter") == Key_State::Pressed)
    {
        if (text_hud.size() > 0)
        {
            current_command = text_hud[text_hud.size() - 1]->get_text();
            execute_command(text_hud[text_hud.size() - 1]->get_text());
        }
        new_line();
    }

    render();
}

// HUD_CLI destructor
HUD_CLI::~HUD_CLI()
{
    unload();
}

// Robot constructor
Robot::Robot(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Object(a_advanced_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
{
    game = (Game*)a_advanced_struct;
}

// Create each robot part
void Robot::create()
{
    Scene* scene = game->get_scene(get_scene_name());

    glm::vec3 axis_scale = glm::vec3(0.3, 0.3, 0.9);
    glm::vec3 screen_scale = glm::vec3(0.1, 1.0, 1.0);
    float screen_y = 1.35f;
    glm::vec3 support_scale = glm::vec3(0.4, 1.0, 0.4);
    float support_y = 0.35f;
    float wheel_offset = 0.4;
    float wheel_y = 0;

    // Create each robot part
    axis = scene->new_object(get_name() + ";axis", "cylinder", get_attached_transform(), glm::vec3(0, wheel_y, 0), glm::vec3(0, 0, 0), axis_scale, false, game->get_assets_directory_path() + "textures/warehouse/robot/wheel.png", false, true, false);
    support = scene->new_object(get_name() + ";support", "one_faced_cube", get_attached_transform(), glm::vec3(0, support_y, 0), glm::vec3(0, 0, 0), support_scale, false, game->get_assets_directory_path() + "textures/warehouse/robot/support.png", false, true, false);
    screen = scene->new_object(get_name() + ";screen", "cube", get_attached_transform(), glm::vec3(0, screen_y, 0), glm::vec3(0, 0, 0), screen_scale, false, get_screen_texture(), false, true, false);

    wheel0 = scene->new_object(get_name() + ";wheel0", "cylinder", axis->get_attached_transform(), glm::vec3(0, 0, -wheel_offset), glm::vec3(0, 0, 0), wheel_scale, false, game->get_assets_directory_path() + "textures/warehouse/robot/wheel.png", false, true, false);
    wheel1 = scene->new_object(get_name() + ";wheel1", "cylinder", axis->get_attached_transform(), glm::vec3(0, 0, wheel_offset), glm::vec3(0, 0, 0), wheel_scale, false, game->get_assets_directory_path() + "textures/warehouse/robot/wheel.png", false, true, false);
}

// Go forward with the robot 
void Robot::forward(float forward_multiplicator)
{
    // Calculate the movement to apply
    glm::vec3 forward = glm::normalize(get_attached_transform()->get_forward() * glm::vec3(forward_multiplicator, forward_multiplicator, forward_multiplicator));
    glm::vec3 movement = forward * glm::vec3(-get_wheel_speed(), get_wheel_speed(), get_wheel_speed());
    get_attached_transform()->move(movement * glm::vec3(game->get_delta_time(), game->get_delta_time(), game->get_delta_time()));

    // Calculate the wheel movement
    float wheel_diameter = (wheel_scale[1]) * 3.1415;
    float angle = (get_wheel_speed() / wheel_diameter * forward_multiplicator) * 2.0;
    wheel0->get_attached_transform()->rotate(glm::vec3(0, 0, angle));
    wheel1->get_attached_transform()->rotate(glm::vec3(0, 0, angle));
}

// Update the robot
void Robot::update()
{
    glm::vec3 screen_rotation = screen->get_attached_transform()->get_rotation();
    screen->get_attached_transform()->set_rotation(glm::vec3(screen_rotation[0], screen_y_rotation, screen_rotation[2]));
    Object::update();
}

// Turn the robot on himself
void Robot::turn(float turn_multiplicator)
{
    // Calculate the rotation of the robot
    glm::vec3 rotation = glm::vec3(0, turn_multiplicator * 3.1415, 0) * glm::vec3(get_wheel_speed(), get_wheel_speed(), get_wheel_speed());
    rotation *= glm::vec3(4 * turning_multiplicator, 4 * turning_multiplicator, 4 * turning_multiplicator);
    glm::vec3 final_rotation = rotation * glm::vec3(game->get_delta_time(), game->get_delta_time(), game->get_delta_time());
    get_attached_transform()->rotate(final_rotation);

    // Turn the wheels
    final_rotation *= glm::vec3(3.1415, 3.1415, 3.1415);
    wheel0->get_attached_transform()->rotate(glm::vec3(0, 0, final_rotation[1]));
    wheel1->get_attached_transform()->rotate(glm::vec3(0, 0, -final_rotation[1]));
}