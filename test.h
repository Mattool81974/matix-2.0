#pragma once

#include "Game.h"
#include <random>

class Ammo : public Object
{
    // Class representing an Ammo
public:
    Ammo(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Ammo constructor
    void late_update(); // Late update the ammo
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

class Target : public Object
{
    // Class representing a Targer
public:
    Target(Advanced_Struct* a_advanced_struct, std::string a_name, std::string a_scene_name, Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic = 0, Physic_Object* a_attached_physic = 0); // Target constructor
    void late_update(); // Update lately the target
    void place_randomly(); // Place randomly the target
    void undeploy(); // Undexploy the target
    void update(); // Update the target
    ~Target(); // Target destructor

    // Getters and setters
    inline float get_bottom_y() { return bottom_y; };
    inline glm::vec2 get_max_pos() { return max_pos; };
    inline glm::vec2 get_min_pos() { return min_pos; };
    inline float get_top_y() { return top_y; };
    inline float get_undeployed_time() { return undeployed_time; };
    inline bool is_deployed() { return deployed; };
    inline void set_bottom_y(float a_y) { bottom_y = a_y; };
    inline void set_max_pos(glm::vec2 a_max_pos) { max_pos = a_max_pos; };
    inline void set_min_pos(glm::vec2 a_min_pos) { min_pos = a_min_pos; };
    inline void set_top_y(float a_y) { top_y = a_y; };
private:
    float bottom_y = 0;
    bool deployed = false;
    glm::vec2 min_pos = glm::vec2(0, 0);
    glm::vec2 max_pos = glm::vec2(10, 10);
    float top_y = 0;
    float undeployed_time = glfwGetTime();
};