#include "test.h"

// Ammo constructor
Ammo::Ammo(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Object(a_advanced_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
{
    game = (Game*)get_game_struct();
    creation_time = glfwGetTime();
}

// Late update the ammo
void Ammo::late_update()
{
    if (get_collisions()->size() > 0)
    {
        Scene* scene = (*game->get_scenes())[get_scene_name()];
        scene->destroy(get_name());
    }
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
    float ammo_speed = 150;
    Ammo* ammo = scene->new_object<Ammo>("ammo-" + std::to_string(ammo_shooted), "ammo", 0, position, rotation, scale, false, "../textures/shell.png", false);
    ammo->get_tags()->push_back("ammo");
    ammo->get_attached_physic_object()->get_collision()->set_height(0.1);
    ammo->get_attached_physic_object()->get_collision()->set_width(0.1);
    ammo->get_attached_physic_object()->set_velocity(glm::vec3(ammo_speed, ammo_speed, ammo_speed) * forward);

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

    if (get_game_struct()->get_right_mouse_button_state()) // If the right button is pressed
    {
        zoom_state = 0;
    }
    else
    {
        zoom_state = 1;
    }
    zoom();
}

// Apply a zoom to the famas
void Famas::zoom()
{
    if (zoom_state == 0)
    {
        get_attached_transform()->set_anchored_position(glm::vec3(-0.0, -0.15f, -0.5));
        get_game_struct()->get_camera()->set_fov(30.0f);
    }
    else
    {
        get_attached_transform()->set_anchored_position(glm::vec3(-0.3, -0.15f, -0.45));
        get_game_struct()->get_camera()->set_fov(45.0f);
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

// Update lately the target
void Target::late_update()
{
    for (int i = 0; i < get_collisions()->size(); i++)
    {
        Object* object1 = (Object *)(*get_collisions())[i].get_object1();
        Object* object2 = (Object*)(*get_collisions())[i].get_object2();

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
    Object::late_update();
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
    int rand_texture = rand() % get_textures()->size();
    int rand_x = get_min_pos()[0] + (rand() % (int)(get_max_pos()[0] - get_min_pos()[0]));
    float rand_y = rand() % 2;
    int rand_z = get_min_pos()[1] + (rand() % (int)(get_max_pos()[1] - get_min_pos()[1]));

    if (rand_y == 0)
    {
        rand_y = get_bottom_y();
    }
    else{
        rand_y = get_top_y();
    }

    deployed = true;
    set_map_pos(glm::vec2(rand_x, rand_z));
    get_attached_graphic_object()->set_texture(get_game_struct()->get_texture((*get_textures())[rand_texture]));
    get_attached_transform()->set_position(glm::vec3(rand_x, rand_y, rand_z));
}

// Undeploy the target
void Target::undeploy()
{
    if (is_deployed())
    {
        deployed = false;
        undeployed_time = glfwGetTime();
    }
}

// Update the target
void Target::update()
{
    if (!is_deployed() && glfwGetTime() - undeployed_time >= 1)
    {
        place_randomly();
   }
}

// Target destructor
Target::~Target()
{
    Object::~Object();
}