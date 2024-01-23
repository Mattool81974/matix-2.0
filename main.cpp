#include "Game.h"
#include <iostream>

class Ammo : public Object
{
    // Class representing an Ammo
public:
    Ammo(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Ammo constructor
    void update(); // Update the ammo
    ~Ammo(); // Ammo destructor
private:
    float creation_time = 0;
    float deadline = 3;

    Game* game = 0;
};

class Famas : public Object
{
    // Class representing a Famas
public:
    Famas(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Famas constructor
    void shoot(); // Shoot with the famas
    void update(); // Update the famas
    void zoom(); // Apply a zoom to shoot
    ~Famas(); // Famas destructor
private:
    unsigned short ammo_by_second = 6;
    unsigned int ammo_shooted = 0;
    float last_ammo_shooted = 0;
    unsigned short zoom_state = 0;

    Game* game = 0;
};

// Ammo constructor
Ammo::Ammo(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Object(a_advanced_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
{
    game = (Game*)get_game_struct();
    creation_time = glfwGetTime();
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

}

// Famas constructor
Famas::Famas(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Object(a_advanced_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
{
    game = (Game*)get_game_struct();
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
    glm::vec3 scale = glm::vec3(0.1, 0.1, 0.3);

    // Create the ammo
    Ammo *ammo = scene->new_object<Ammo>("ammo-" + std::to_string(ammo_shooted), "ammo", 0, position, rotation, scale, false, "../textures/shell.png", false);
    ammo->get_attached_physic_object()->set_velocity(glm::vec3(50, 50, 50) * forward);

    // Update shoot datas
    ammo_shooted += 1;
    last_ammo_shooted = glfwGetTime();
}

// Update the famas
void Famas::update()
{
    if (get_game_struct()->get_left_mouse_button_state()) // If the left button is pressed
    {
        if (glfwGetTime() - last_ammo_shooted > 1.0f/(float)ammo_by_second)
        {
            shoot();
        }
    }
    
    if (get_game_struct()->get_right_mouse_button_state()) // If the right button is pressed
    {
        zoom();
    }
}

// Apply a zoom to the famas
void Famas::zoom()
{
    if (zoom_state == 0)
    {
        get_attached_transform()->set_anchored_position(glm::vec3(-0.0, -0.275f, -0.5));
        get_game_struct()->get_camera()->set_fov(30.0f);
        zoom_state = 1;
    }
    else
    {
        get_attached_transform()->set_anchored_position(glm::vec3(-0.35, -0.25f, -0.5));
        get_game_struct()->get_camera()->set_fov(45.0f);
        zoom_state = 0;
    }
}

// Famas destructor
Famas::~Famas()
{

}

int main()
{
    // Construct game
    Game game(1600, 900);
    Camera* camera = game.get_camera();
    game.new_vao("../vbos/famas.vbo", "famas");
    game.new_vao("../vbos/shell.vbo", "ammo");
    game.new_part(1, "cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 3, 1), "../textures/wall.png");
    game.new_part(2, "cube", glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 3, 1), "../textures/pillar.png");

    // Construct scene
    Scene* scene = game.new_scene("level0", "../maps/level0.wad");
    game.set_current_scene("level0");

    // Construct objects for testing
    Object *player = scene->new_object("player", "player", 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, "", false, false, true);
    // scene->new_object("sol", "square", 0, glm::vec3(12, 0, 12), glm::vec3(90, 0, 0), glm::vec3(25, 25, 1), true, "../textures/floor.png");
    // scene->new_object("locker", "cube", 0, glm::vec3(2, 1, 2), glm::vec3(0, 0, 0), glm::vec3(1, 2, 1), true, "../textures/locker.png", false);
    // scene->new_object("computer", "cube", 0, glm::vec3(2, 0.5, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), true, "../textures/computer.png", false);
    // scene->new_object("table", "table", 0, glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), true, "../textures/table.png", false);
    // scene->new_object("chair", "chair", 0, glm::vec3(1, 0.5, 0), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), true, "../textures/chair.png", false);
    // scene->new_object("clock", "circle", 0, glm::vec3(4.49, 2.25, 0), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), true, "../textures/clock.png", false);
    Famas *famas = scene->new_object<Famas>("famas", "famas", camera, glm::vec3(0, 0, 0), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), true, "../textures/famas.png", false, true, false);
    // Object *luxary_famas = scene->new_object("luxary_famas", "famas", 0, glm::vec3(0, 3, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), true, "../textures/luxary_famas.png", false, true, false);
    
    // Configurate some objects in the scene
    camera->set_parent(player->get_attached_transform());
    famas->get_attached_transform()->set_anchored_position(glm::vec3(-0.35, -0.25f, -0.5));
    famas->get_attached_transform()->set_parent_rotation_multiplier(glm::vec3(1.0f, -1.0f, 1.0f));
    player->get_attached_physic_object()->get_collision()->set_width(0.4);

    while (game.run())
    {
        game.update_event();

        game.update();
    }

    return 0;
}