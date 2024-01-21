#include "Game.h"
#include <iostream>

class Ammo : public Object
{
    // Class representing an Ammo
public:
    Ammo(Advanced_Struct* a_advanced_struct, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Ammo constructor
    void update(); // Update the ammo
    ~Ammo(); // Ammo destructor
private:
    float creation_time = 0;
    float deadline = 3;
};

class Famas : public Object
{
    // Class representing a Famas
public:
    Famas(Advanced_Struct* a_advanced_struct, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Famas constructor
    void shoot(); // Shoot with the famas
    void update(); // Update the famas
    ~Famas(); // Famas destructor
private:
    unsigned short ammo_by_second = 6;
    unsigned int ammo_shooted = 0;
    float last_ammo_shooted = 0;

    Game* game = 0;
};

// Ammo constructor
Ammo::Ammo(Advanced_Struct* a_advanced_struct, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Object(a_advanced_struct, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
{
    creation_time = glfwGetTime();
}

// Update the ammo
void Ammo::update()
{
    
}

// Ammo destructor
Ammo::~Ammo()
{

}

// Famas constructor
Famas::Famas(Advanced_Struct* a_advanced_struct, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Object(a_advanced_struct, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
{
    game = (Game*)get_game_struct();
}

// Shoot with the famas
void Famas::shoot()
{
    Scene* scene = (*game->get_scenes())[get_scene_name()];

    // Calculate ammo transformation
    glm::vec3 forward = glm::normalize(get_attached_transform()->get_forward());
    forward = glm::cross(forward, glm::vec3(0, -1, 0));
    forward = glm::vec3(-forward[0], forward[1], forward[2]);

    glm::vec3 position = get_attached_transform()->get_absolute_position() + glm::vec3(0, 0.67, 0.0) + glm::vec3(0.2, 0, 0.2) * forward;
    glm::vec3 rotation = get_attached_transform()->get_rotation() + glm::vec3(0, 180, 0);
    glm::vec3 scale = glm::vec3(0.1, 0.1, 0.3);

    // Create the ammo
    Ammo *ammo = scene->new_object<Ammo>("ammo-" + std::to_string(ammo_shooted), "ammo", 0, position, rotation, scale, "../textures/shell.png", false);
    ammo->get_attached_physic_object()->set_velocity(glm::vec3(50, 0, 50) * forward);

    // Update shoot datas
    ammo_shooted += 1;
    last_ammo_shooted = glfwGetTime();
}

// Update the famas
void Famas::update()
{
    if (get_game_struct()->get_key_state("a")) // If the A (AZERTY) key is pressed
    {
        if (glfwGetTime() - last_ammo_shooted > 1.0f/(float)ammo_by_second)
        {
            shoot();
        }
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
    Object *player = scene->new_object("player", "player", 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "", false, false, true);
    scene->new_object("sol", "square", 0, glm::vec3(12, 0, 12), glm::vec3(90, 0, 0), glm::vec3(25, 25, 1), "../textures/floor.png");
    scene->new_object("locker", "cube", 0, glm::vec3(2, 1, 2), glm::vec3(0, 0, 0), glm::vec3(1, 2, 1), "../textures/locker.png", false);
    scene->new_object("computer", "cube", 0, glm::vec3(2, 0.5, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/computer.png", false);
    scene->new_object("table", "table", 0, glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/table.png", false);
    scene->new_object("chair", "chair", 0, glm::vec3(1, 0.5, 0), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), "../textures/chair.png", false);
    scene->new_object("clock", "circle", 0, glm::vec3(4.49, 2.25, 0), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), "../textures/clock.png", false);
    Famas *famas = scene->new_object<Famas>("famas", "famas", player->get_attached_transform(), glm::vec3(1, -0.9, 0.35), glm::vec3(0, 270, 0), glm::vec3(1, 1, 1), "../textures/famas.png", false);
    scene->new_object("luxary_famas", "famas", 0, glm::vec3(0, 3, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "../textures/luxary_famas.png", false);
    Ammo *ammo = scene->new_object<Ammo>("ammo", "ammo", 0, glm::vec3(1, 3.6725, -0.03), glm::vec3(0, 180, 0), glm::vec3(0.01, 0.01, 0.03), "../textures/shell.png", false);
    
    // Configurate some objects in the scene
    camera->set_parent(player->get_attached_transform());

    game.run();

    return 0;
}