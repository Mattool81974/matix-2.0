#include "../headers/lunar_rover.h"

namespace Lunar_Rover
{
    // Main game variables
    Camera* camera = 0;
    Game* game = 0;
    Scene* lunar_scene = 0;

    // Rover constructor
    Rover::Rover(Advanced_Struct* a_advanced_struct, std::string a_name , std::string a_scene_name , Transform_Object* a_attached_transform, Graphic_Object* a_attached_graphic, Physic_Object* a_attached_physic) : Object(a_advanced_struct, a_name, a_scene_name, a_attached_transform, a_attached_graphic, a_attached_physic)
    {

    }

    // Create the rover part by part
    void Rover::create()
    {
        // Create main variables for creation
        Scene* scene = game->get_scene(get_scene_name());
        std::string texture_head = game->get_assets_directory_path() + "textures/lunar_rover/rover/head.png";
        std::string texture_neck = game->get_assets_directory_path() + "textures/lunar_rover/rover/neck.png";
        std::string texture_support = game->get_assets_directory_path() + "textures/lunar_rover/rover/support.png";
        std::string texture_wheel = game->get_assets_directory_path() + "textures/lunar_rover/rover/wheel.png";
        std::string texture_wheel_base = game->get_assets_directory_path() + "textures/lunar_rover/rover/wheel_support.png";
        std::string texture_wheel_support = game->get_assets_directory_path() + "textures/lunar_rover/rover/wheel_support.png";
        Transform_Object* transform = get_attached_transform();

        // Create the support
        glm::vec3 support_position = glm::vec3(0.0, 1.5, 0.0);
        glm::vec3 support_scale = glm::vec3(5.0, 0.5, 2.5);
        support = scene->new_object(get_name() + ";support", "cube", transform, support_position, glm::vec3(0, 0, 0), support_scale, false, texture_support, false, true, false);

        // Create the neck of the rover
        glm::vec3 neck_position = glm::vec3(0.0, 1.0, -0.75);
        glm::vec3 neck_scale = glm::vec3(0.2, 1.5, 0.2);
        neck = scene->new_object(get_name() + ";neck", "one_faced_cube", support->get_attached_transform(), neck_position, glm::vec3(0, 0, 0), neck_scale, false, texture_neck, false, true, false);
        neck->get_attached_transform()->set_anchored_position(glm::vec3(0, -0.75, 0));

        // Create the head of the rover
        glm::vec3 head_position = glm::vec3(0.0, 0.95, 0.0);
        glm::vec3 head_scale = glm::vec3(0.2, 0.4, 0.8);
        head = scene->new_object(get_name() + ";head", "cube", neck->get_attached_transform(), head_position, glm::vec3(0, 0, 0), head_scale, false, texture_head, false, true, false);

        // Create the wheels
        float wheel_separation = 3.0f;
        float x_middle = -1.5;
        for (int i = 0; i < get_wheels_number() / 2.0; i++)
        {
            // Create the wheels variables for creation
            float x = x_middle - ((- (wheel_separation * (get_wheels_number() / 2.0)) / 2.0) + (i / (get_wheels_number() / 2.0)) * (wheel_separation * (get_wheels_number() / 2.0)));
            glm::vec3 base_scale = glm::vec3(0.2, 1.0, 0.2);
            glm::vec3 position_1 = glm::vec3(x, -0.75, 1);
            glm::vec3 position_2 = glm::vec3(x, -0.75, -1);
            glm::vec3 rotation = glm::vec3(0, 0, 0);
            glm::vec3 support_offset = glm::vec3(0.0, -0.4, 0.25);
            glm::vec3 support_scale = glm::vec3(0.2, 0.2, 0.5);
            glm::vec3 wheel_offset = glm::vec3(0, 0, 0.25);
            glm::vec3 wheel_scale = glm::vec3(1, 1, 0.2);

            // Create the base support
            Object* wheel_base_0 = scene->new_object(get_name() + ";wheel_base" + std::to_string(i * 2), "one_faced_cube", support->get_attached_transform(), position_1, rotation, base_scale, false, texture_wheel_base, false, true, false);
            Object* wheel_base_1 = scene->new_object(get_name() + ";wheel_base" + std::to_string(i * 2 + 1), "one_faced_cube", support->get_attached_transform(), position_2, rotation, base_scale, false, texture_wheel_base, false, true, false);

            // Create the wheel support
            Object* wheel_support_0 = scene->new_object(get_name() + ";wheel_support" + std::to_string(i * 2), "one_faced_cube", wheel_base_0->get_attached_transform(), support_offset, rotation, support_scale, false, texture_wheel_support, false, true, false);
            Object* wheel_support_1 = scene->new_object(get_name() + ";wheel_support" + std::to_string(i * 2 + 1), "one_faced_cube", wheel_base_1->get_attached_transform(), support_offset * glm::vec3(1.0, 1.0, -1.0), rotation, support_scale, false, texture_wheel_support, false, true, false);

            // Create the wheel
            Object* wheel0 = scene->new_object(get_name() + ";wheel" + std::to_string(i * 2), "cylinder", wheel_support_0->get_attached_transform(), wheel_offset, rotation, wheel_scale, false, texture_wheel, false, true, false);
            Object* wheel1 = scene->new_object(get_name() + ";wheel" + std::to_string(i * 2 + 1), "cylinder", wheel_support_1->get_attached_transform(), wheel_offset * glm::vec3(1.0, 1.0, -1.0), rotation, wheel_scale, false, texture_wheel, false, true, false);
        
            // Configurate the wheels
            wheels_base.push_back(wheel_base_0);
            wheels_base.push_back(wheel_base_1);
            wheels_support.push_back(wheel_support_0);
            wheels_support.push_back(wheel_support_1);
            wheels.push_back(wheel0);
            wheels.push_back(wheel1);
        }
    }

    // Move the rover forward
    void Rover::forward(float multiplier)
    {
        // Move the transform
        float speed = 2.0f * game->get_delta_time();
        get_attached_transform()->move(glm::vec3(speed * multiplier, 0, 0));

        // Animate the wheels
        for (int i = 0; i < wheels.size(); i++)
        {
            Object* wheel = wheels[i];
            float speed = 180;
            wheel->get_attached_transform()->rotate(glm::vec3(0, 0, speed * game->get_delta_time() * -multiplier));
        }
    }

    // Deploy the rover
    void Rover::deploy(bool instant)
    {
        if (!is_deployed())
        {
            deployed = true;

            // Calculate final transformation
            float deployment_time = 5.0f;
            float neck_deployment = deployment_time;
            glm::vec3 neck_base_rotation = glm::vec3(90, 0, 0);
            glm::vec3 neck_final_rotation = glm::vec3(0, 0, 0);

            neck->get_attached_transform()->start_animation();
            if (instant) // Apply the transformation
            {
                neck->get_attached_transform()->set_rotation(neck_final_rotation);
            }
            else
            {
                // Apply the neck animation
                float state = 1.0;
                if (neck->get_attached_transform()->get_animations()->size() > 0)
                {
                    state = neck->get_attached_transform()->get_current_animation()->state / neck->get_attached_transform()->get_current_animation()->duration;
                }
                neck->get_attached_transform()->reset_animation();
                neck->get_attached_transform()->add_rotation_animation(neck_deployment, neck_base_rotation, neck_final_rotation);
                neck->get_attached_transform()->get_current_animation()->state = (1.0 - state) * neck_deployment;
            }
        }
    }

    // Set the back view for the camera
    void Rover::set_back_view()
    {
        camera->set_parent(get_attached_transform());
        camera->set_position(get_camera_back_view_offset());
    }

    // Set the ehad view for the camera
    void Rover::set_head_view()
    {
        camera->set_parent(head->get_attached_transform());
        camera->set_position(get_camera_head_view_offset());
    }

    // Turn the head of the rover
    void Rover::turn_head(float multiplier)
    {
        // Calculate the necessary stuff
        float speed = turn_head_speed * multiplier * game->get_delta_time();

        // Turn the head
        neck->get_attached_transform()->rotate(glm::vec3(0, speed, 0));
        std::cout << "A " << neck->get_attached_transform()->get_up()[0] << " " << neck->get_attached_transform()->get_up()[1] << " " << neck->get_attached_transform()->get_up()[2] << std::endl;
    }

    // Undeploy the rover
    void Rover::undeploy(bool instant)
    {
        if (is_deployed())
        {
            deployed = false;

            // Calculate final transformation
            float undeployment_time = 5.0f;
            glm::vec3 neck_base_rotation = glm::vec3(0, 0, 0);
            glm::vec3 neck_final_rotation = glm::vec3(90, 0, 0);
            float neck_undeployment = undeployment_time;

            if (instant)
            {
                neck->get_attached_transform()->set_rotation(neck_final_rotation);
            }
            else
            {
                // Apply neck animation
                float state = 1.0;
                if (neck->get_attached_transform()->get_animations()->size() > 0)
                {
                    state = neck->get_attached_transform()->get_current_animation()->state / neck->get_attached_transform()->get_current_animation()->duration;
                }
                neck->get_attached_transform()->reset_animation();
                neck->get_attached_transform()->add_rotation_animation(neck_undeployment, neck_base_rotation, neck_final_rotation);
                neck->get_attached_transform()->get_current_animation()->state = (1.0 - state) * neck_undeployment;
            }
        }
    }

    // Update the rover
    void Rover::update()
    {
        if (game->get_key_state_frame("z"))
        {
            forward(1);
        }
        if (game->get_key_state_frame("s"))
        {
            forward(-1);
        }

        // Control the head
        if (game->get_key_state_frame("left arrow"))
        {
            turn_head();
        }
        if (game->get_key_state_frame("right arrow"))
        {
            turn_head(-1);
        }

        // Control the deployment
        if (game->get_key_state_frame("l"))
        {
            undeploy();
        }
        if (game->get_key_state_frame("o"))
        {
            deploy();
        }

        Object::update();
    }

    // Advanced game variables
    Rover* rover = 0;

    // Load the lunar environment
    void load_lunar_environment()
    {
        // Configure the game
        camera = game->get_camera();

        // Add the needed parts
        Part* floor = game->new_part(1, "one_faced_cube", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), game->get_assets_directory_path() + "textures/lunar_rover/floor.png");

        // Create the lunar scene
        lunar_scene = game->new_scene("moon", game->get_assets_directory_path() + "maps/moon.wad", Map_Opening_Mode::Complex);

        // Create the exterior objects
        rover = lunar_scene->new_object<Rover>("rover", "", 0, glm::vec3(5, 2, 10), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, "", false, false, true);
        rover->get_attached_physic_object()->set_elasticity(0);
        rover->create();
    }

    // Setup the lunar environment
    void set_lunar_environment()
    {
        // Set some game attributes
        game->set_background_color(glm::vec4(0, 0, 0, 1.0));
        game->set_current_scene("moon");
        rover->set_back_view();
    }

    // Run the lunar rover game
    void run_lunar_rover(std::string path)
    {
        game = new Game(1600, 900, path, false);
        game->load_from_config_file("../matix_config.cfg");
        game->load_VAOs();

        load_lunar_environment();
        set_lunar_environment();

        while (game->run())
        {
            game->update_event();
            game->update();

            if (game->get_key_state_frame("p") == Key_State::Pressed)
            {
                rover->set_back_view();
            }

            if (game->get_key_state_frame("m") == Key_State::Pressed)
            {
                rover->set_head_view();
            }

            game->render();
        }

        delete game;
        game = 0;
    }
}