#include "../headers/player.h"

// Camera_Handler constructor
Player::Camera_Handler::Camera_Handler(Advanced_Struct* advanced_struct, std::string name, std::string scene_name, Transform_Object* attached_transform, Graphic_Object* attached_graphic, Physic_Object* attached_physic) : Object(advanced_struct, name, scene_name, attached_transform, attached_graphic, attached_physic)
{
    // Load the main global datas
    a_game = (Game*)get_game_struct();
    a_camera = a_game->get_camera();
    a_camera->set_parent(get_attached_transform());
}

// Clone the Camera_Handler
void* Player::Camera_Handler::clone(Advanced_Struct* game_struct, std::string name, std::string scene_name, Transform_Object* attached_transform, Graphic_Object* attached_graphic, Physic_Object* attached_physic)
{
    return new Camera_Handler(game_struct, name, scene_name, attached_transform, attached_graphic, attached_physic);
}

// Late update the player
void Player::Camera_Handler::late_update()
{
    update_move();
}

// Update the moving of the camera handler
void Player::Camera_Handler::update_move()
{
    float delta_time = get_game_struct()->get_delta_time();
    float speed = get_speed();
    speed *= delta_time;

    // Rotate and move camera
    if (can_rotate())
    {
        float rotate_speed = 45;
        float sensitivity = get_game_struct()->get_camera()->get_sensitivity();
        get_attached_transform()->rotate_plan(glm::vec3(0.0, sensitivity * delta_time * get_game_struct()->get_mouse_move_x(), 0.0));
        get_attached_transform()->rotate_plan(glm::vec3(-sensitivity * get_game_struct()->get_delta_time() * get_game_struct()->get_mouse_move_y(), 0.0, 0.0));
    }

    // Move the Camera_Handler
    if (can_move())
    {
        float jump_force = 4;
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
}

// Camera_Handler destructor
Player::Camera_Handler::~Camera_Handler()
{

}