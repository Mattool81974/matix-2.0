#include "test.h"

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
    if (get_collisions_result()->size() > 0)
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
    glm::vec3 scale = glm::vec3(0.1, 0.1, 0.3);

    // Create the ammo
    float ammo_speed = get_ammo_speed();
    Ammo* ammo = scene->new_object<Ammo>("ammo-" + std::to_string(ammo_shooted), "ammo", 0, position, rotation, scale, false, "../textures/shell.png", false);
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
    // std::cout << "Absolute position " << get_attached_transform()->get_absolute_position()[0] << " " << get_attached_transform()->get_absolute_position()[1] << " " << get_attached_transform()->get_absolute_position()[2] << std::endl;
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
        Object* object1 = (Object *)get_collisions_result()->at(i)->object1;
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
    else{
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
    if (game->get_key_state("e")) open();
    else close();

    Object::update();
}

// Door destructor
Door::~Door()
{
    Object::~Object();
}