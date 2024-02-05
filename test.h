#pragma once

#include "Game.h"

class Entity : public Object
{
    // Class representing an entity
public:
    Entity(Advanced_Struct* a_advanced_struct = 0, std::string a_name = "", std::string a_scene_name = "", Transform_Object* a_attached_transform = 0, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Entity constructor
    void* clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Clone the entity
    ~Entity(); // Entity destructor

    // Getters and setters
    inline float get_speed() { return speed; };
    inline bool is_moving() { return moving; };
    void update() {}; // Update the entity
protected:
    bool moving = true; // If the entity can move or not
    float speed = 5.0f; // Speed of the entity
private:
    Base_Struct* base_struct = 0; // Base struct of the game
};

class Player : public Entity
{
    // Class representing a player entity
public:
    Player(Advanced_Struct* a_advanced_struct = 0, std::string a_name = "", std::string a_scene_name = "", Transform_Object* a_attached_transform = 0, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Player constructor
    void* clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Clone the player
    void late_update(); // Late update the player
    void update_move(); // Update the moving of the player
    ~Player(); // Player destructor
private:
    Game* game = 0;
};

class Ammo : public Object
{
    // Class representing an Ammo
public:
    Ammo(Advanced_Struct* a_advanced_struct = 0, std::string a_name = "", std::string a_scene_name = "", Transform_Object* a_attached_transform = 0, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Ammo constructor
    void* clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Clone the ammo
    void last_update(); // Late update the ammo
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
    Famas(Advanced_Struct* a_advanced_struct = 0, std::string a_name = "", std::string a_scene_name = "", Transform_Object* a_attached_transform = 0, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Famas constructor
    void* clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Clone the famas
    void shoot(); // Shoot with the famas
    void update(); // Update the famas
    void zoom(); // Apply a zoom to shoot
    ~Famas(); // Famas destructor

    // Getters and setters
    inline unsigned short get_ammo_by_second() { return ammo_by_second; };
    inline float get_ammo_speed() { return ammo_speed; };
    inline float get_step_back_duration() { return step_back_duration; };
    inline glm::vec3 get_step_back_force() { return step_back_force; };
    inline float get_zoom_duration() { return zoom_duration; };
    inline float get_zoom_fov() { return zoom_fov; };
    inline float get_zoom_normal_fov() { return zoom_normal_fov; };
private:
    unsigned short ammo_by_second = 6; // Number of ammo shot by second
    unsigned int ammo_shooted = 0; // Time when the last ammo was shot
    float ammo_speed = 150; // Spedd of an ammo
    float last_ammo_shooted = 0; // Time when the last-last ammo was shot
    float step_back_duration = 1 / (6 * 2.1); // Duration of a step back
    glm::vec3 step_back_force = glm::vec3(0.005, 0.005, 0.03); // Force of a step back
    float zoom_duration = 0.05; // Duration of a zoom
    float zoom_fov = 30; // FOV for the zoom at 2
    float zoom_normal_fov = 45; // FOV for the normal zoom at 1
    float zoom_state = 0; // State of zoom of the weapon

    Game* game = 0;
};

class Target : public Object
{
    // Class representing a Targer
public:
    Target(Advanced_Struct* a_advanced_struct = 0, std::string a_name = "", std::string a_scene_name = "", Transform_Object* a_attached_transform = 0, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Target constructor
    void* clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Clone the target
    void last_update(); // Update lately the target
    void new_texture(std::string texture); // Add a texture to the target
    void place_randomly(); // Place randomly the target
    void undeploy(); // Undexploy the target
    void update(); // Update the target
    ~Target(); // Target destructor

    // Getters and setters
    inline float get_bottom_y() { return bottom_y; };
    inline glm::vec2 get_max_pos() { return max_pos; };
    inline glm::vec2 get_min_pos() { return min_pos; };
    inline std::vector<std::string>* get_textures() { return &textures; };
    inline float get_top_y() { return top_y; };
    inline float get_undeployed_time() { return undeployed_time; };
    inline bool is_deployed() { return deployed; };
    inline void set_bottom_y(float a_y) { bottom_y = a_y; };
    inline void set_max_pos(glm::vec2 a_max_pos) { max_pos = a_max_pos; };
    inline void set_min_pos(glm::vec2 a_min_pos) { min_pos = a_min_pos; };
    inline void set_top_y(float a_y) { top_y = a_y; };
private:
    float bottom_y = 0; // Bottom Y pos of the target
    bool deployed = false; // If the target is deployed or not
    float deployement_space = 0.5; // Time between undeployment and deployement
    float deployement_time = 0.2; // Length of the deployement
    glm::vec2 min_pos = glm::vec2(0, 0); // Minimum pos of the target
    glm::vec2 max_pos = glm::vec2(10, 10); // Maximum pos of the target
    float top_y = 0; // Top Y pos of the target
    float undeployed_time = glfwGetTime(); // Time at the last undeployement of the target

    std::vector<std::string> textures = std::vector<std::string>(); // Vector of all the possible texture for a target
};

class Door : public Object
{
    // Class representing a door
public:
    Door(Advanced_Struct* a_advanced_struct = 0, std::string a_name = "", std::string a_scene_name = "", Transform_Object* a_attached_transform = 0, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Door constructor
    void after_loading(); // Function called after the loading of the scene
    void* clone(Advanced_Struct* a_game_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Clone the door
    void close(); // Close the door
    void open(); // Open the door
    void update(); // Update the door
    ~Door(); // Door destructor

    // Getters and setters
    inline float get_animation_time() { return animation_time; };
    inline bool is_first_door() { return first_door; };
    inline bool is_horizontal() { return horizontal; };
    inline bool is_linked() { return linked; };
    inline bool is_opened() { return opened; };
    inline float get_close_y_rotation()
    {
        float multiplier = 1; // Calculate the multiplier for the rotation
        if (first_door) multiplier = -1;

        float value = 90; // Calculate the value of the rotation
        if (!is_horizontal()) { value = 180; if (multiplier == 1) multiplier = 0; }

        float final_value_rotation = value * multiplier;
        if (!is_horizontal()) final_value_rotation = normalize_rotation(glm::vec3(0, final_value_rotation, 0))[1];

        return final_value_rotation;
    };
    inline float get_open_multiplier() { return open_multiplier; };
    inline float get_open_y_rotation()
    {
        float multiplier = get_open_multiplier(); // Calculate the multiplier for the rotation

        float value = 90; // Calculate the value of the rotation
        if (is_horizontal()) { value = 180; if (multiplier == 1) multiplier = 0; }

        float final_value_rotation = value * multiplier;
        if (!is_horizontal()) final_value_rotation = normalize_rotation(glm::vec3(0, final_value_rotation, 0))[1];

        return final_value_rotation;
    };
    inline Door* get_other_door() { return other_door; };
    inline float get_wall_offset() { return wall_offset; };
private:
    float animation_time = 0.2; // Length of the animation
    bool first_door = true; // If this door is the first door or not
    bool horizontal = true; // If the door system is horizontal or not
    bool linked = false; // If the door is linked with another or not
    bool opened = true; // If the door is open or not
    float open_multiplier = 1; // Multiplier for the opening rotation
    float wall_offset = 0.45f; // Offset of the door to the wall

    Game* game = 0; // Pointer to the game
    Door* other_door = 0; // Door linked with this door
};

class HUD_CLI : public HUD
{
public:
    HUD_CLI(Game* a_advanced_struct, std::string a_name); // HUD_CLI constructor
    ~HUD_CLI(); // HUD_CLI destructor
private:
    HUD_Text* cli_input = 0; // Pointer to the text input of the CLI
};