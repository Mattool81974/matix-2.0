#include "Game.h"
#include "model.h"
#include "matix/stb_image.h"

double global_mouse_x = 500; // Global variable representing the X pos of the mouse
double global_mouse_y = 500; // Global variable representing the Y pos of the mouse
std::map<std::string, unsigned int> keys = std::map<std::string, unsigned int>(); // Map of each keys in the game

// Compare the 2 objects with their depths
bool compare_depht_hud_object(HUD_Object* a, HUD_Object* b)
{
    return a->get_position()[2] < b->get_position()[2];
}

// Callback function for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Callback function for mouse moving
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    global_mouse_x = xpos;
    global_mouse_y = ypos;
}

// HUD constructor
HUD::HUD(Advanced_Struct* a_advanced_struct, std::string a_name): advanced_struct(a_advanced_struct), name(a_name)
{

}

// Add an existing HUD to the hud
void HUD::add_hud_object(std::string name, HUD_Object* object)
{
    if (contains_hud_object(name)) { std::cout << "HUD \"" << get_name() << "\" : error ! The HUD object \"" << name << "\" you want to add already exists." << std::endl; return; }
    (*get_hud_objects())[name] = object;
    get_sorted_hud_objects()->push_back(object);
    sort_objects();
}

// Return if the HUD contains an HUD Object
bool HUD::contains_hud_object(std::string name)
{
    std::map < std::string, HUD_Object*>* objects = get_hud_objects();
    for (std::map<std::string, HUD_Object*>::iterator it = objects->begin(); it != objects->end(); it++)
    {
        if (it->first == name) { return true; } // Verify each hud object name (first element of map)
    }
    return false;
}

// Create a new HUD Object into the HUD
HUD_Object* HUD::new_hud_object(std::string name, std::string texture_path)
{
    if (contains_hud_object(name)) { std::cout << "HUD \"" << get_name() << "\" error ! The objects \"" << name << "\" you want to create already exists." << std::endl; return 0; }

    bool texture_resize = false; // Load the texture
    Texture* texture = get_advanced_struct()->get_texture(texture_path, texture_resize);

    // Load the VAO
    VAO* vao = (*get_advanced_struct()->get_all_vaos())[(*get_advanced_struct()->get_type())["hud"]];

    HUD_Object* new_object = new HUD_Object(get_advanced_struct(), name, texture, vao);
    add_hud_object(name, new_object);
    return new_object;
}

// Render the HUD
void HUD::render()
{
    for (int i = 0; i < get_sorted_hud_objects()->size(); i++)
    {
        (*get_sorted_hud_objects())[i]->render();
    }
}

// Sort the HUD objects for a good render
void HUD::sort_objects()
{
    std::sort(sorted_hud_objects.begin(), sorted_hud_objects.end(), compare_depht_hud_object); //*/
}

// HUD destructor
HUD::~HUD()
{
    get_sorted_hud_objects()->clear();
    std::map < std::string, HUD_Object*>* objects = get_hud_objects();
    for (std::map<std::string, HUD_Object*>::iterator it = objects->begin(); it != objects->end(); it++)
    {
        delete it->second; // Delete each HUD_Object
        it->second = 0;
    }
}

// Game constructor
Game::Game(int a_window_width, int a_window_height): Advanced_Struct(global_mouse_x, global_mouse_y), window_height(a_window_height), window_width(a_window_width)
{
    load_keys();
    // Configurate base_struct
    get_camera()->set_position(glm::vec3(0.0, 0.0, 0.0));
    get_camera()->set_rotation(glm::vec3(0.0, 0.0, 0.0));
    set_window_height(window_height);
    set_window_width(window_width);

    // GLFW Window generation
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(get_window_width(), get_window_height(), "Fenêtre OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Glad loading
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }

    // Texture setting
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    stbi_set_flip_vertically_on_load(true);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    load_VAOs();
}

// Add an existing HUD to the game
void Game::add_hud(std::string name, HUD* hud)
{
    if (contains_hud(name)) { std::cout << "Matix game : error ! The HUD \"" << name << "\" you want to add already exists." << std::endl; return; }
    (*get_huds())[name] = hud;
}

// Add an existing scene to the game
void Game::add_scene(std::string name, Scene* scene)
{
    if (contains_scene(name)) { std::cout << "Matix game : error ! The scene \"" << name << "\" you want to add already exists." << std::endl; return; }
    (*get_scenes())[name] = scene;
}

// Return if the game contains an HUD Object
bool Game::contains_hud(std::string name)
{
    std::map < std::string, HUD*>* objects = get_huds();
    for (std::map<std::string, HUD*>::iterator it = objects->begin(); it != objects->end(); it++)
    {
        if (it->first == name) { return true; } // Verify each hud object name (first element of map)
    }
    return false;
}

// Returns if the game contains a scene
bool Game::contains_scene(std::string name)
{
    std::map<std::string, Scene*> *scenes = get_scenes();
    for (std::map<std::string, Scene*>::iterator it = scenes->begin(); it != scenes->end(); it++)
    {
        if (it->first == name) { return true; } // Verify each scene name (first element of map)
    }
    return false;
}

// Load the keys in the game
void Game::load_keys()
{
    // Alphabet
    keys["a"] = GLFW_KEY_Q;
    keys["b"] = GLFW_KEY_B;
    keys["c"] = GLFW_KEY_C;
    keys["d"] = GLFW_KEY_D;
    keys["e"] = GLFW_KEY_E;
    keys["f"] = GLFW_KEY_F;
    keys["g"] = GLFW_KEY_G;
    keys["h"] = GLFW_KEY_H;
    keys["i"] = GLFW_KEY_I;
    keys["j"] = GLFW_KEY_J;
    keys["k"] = GLFW_KEY_K;
    keys["l"] = GLFW_KEY_L;
    keys["m"] = GLFW_KEY_SEMICOLON;
    keys["n"] = GLFW_KEY_N;
    keys["o"] = GLFW_KEY_O;
    keys["p"] = GLFW_KEY_P;
    keys["q"] = GLFW_KEY_A;
    keys["r"] = GLFW_KEY_R;
    keys["s"] = GLFW_KEY_S;
    keys["t"] = GLFW_KEY_T;
    keys["u"] = GLFW_KEY_U;
    keys["v"] = GLFW_KEY_V;
    keys["w"] = GLFW_KEY_Z;
    keys["x"] = GLFW_KEY_X;
    keys["y"] = GLFW_KEY_Y;
    keys["z"] = GLFW_KEY_W;

    // Other
    keys["left shift"] = GLFW_KEY_LEFT_SHIFT;
    keys["space"] = GLFW_KEY_SPACE;
    keys["tab"] = GLFW_KEY_TAB;
}

// Create a new HUD Object into the game
HUD* Game::new_hud(std::string name)
{
    if (contains_scene(name)) { std::cout << "Matix game : error ! The objects \"" << name << "\" you want to create already exists." << std::endl; return 0; }

    HUD* new_object = new HUD(this, name);
    add_hud(name, new_object);
    return new_object;
}

// Create a scene into the game and return it
Scene* Game::new_scene(std::string name, std::string map_path, Map_Opening_Mode mode, bool use_graphic, bool use_physic)
{
    if (contains_scene(name)) { std::cout << "Matix game : error ! The scene \"" << name << "\" you want to create already exists." << std::endl; return 0; }

    Scene* new_scene = new Scene(this, name, map_path, use_graphic, use_physic, mode);
    add_scene(name, new_scene);
    return new_scene;
}

// Run the game by doing multiples call to update
bool Game::run()
{
    bool to_return = glfwWindowShouldClose(window);

    if(to_return)
        glfwTerminate();
    return !to_return;
}

// Set the current HUD in the game
void Game::set_current_hud(std::string a_name)
{
    if (contains_hud(a_name))
    {
        current_hud = a_name;
    }
    else
    {
        std::cout << "Matrix game : error ! The current hud \"" << a_name << "\" does not exist." << std::endl;
    }
};

// Set the current scene in the game
void Game::set_current_scene(std::string a_name)
{
    if (contains_scene(a_name))
    {
        current_scene = a_name;
        get_current_scene()->load();
    }
    else
    {
        std::cout << "Matrix game : error ! The current scene \"" << a_name << "\" does not exist." << std::endl;
    }
};

// Update one frame of the game
void Game::update()
{
    if (contains_scene(get_current_scene_name()))
    {
        Scene* scene = get_current_scene();
        if(scene != 0) scene->update();
        
        HUD* hud = get_current_hud();
        if (hud != 0) hud->render();
    }
    else
    {
        std::cout << "Matrix game : error ! The current scene \"" << get_current_scene_name() << "\" does not exist." << std::endl;
    }

    // Update OpenGL
    glfwSwapBuffers(window);
    glfwPollEvents();
}

// Update the event of the game during this frame
void Game::update_event()
{
    // Clear OpenGL window
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate delta time
    set_delta_time(glfwGetTime() - last_frame_time);
    last_frame_time = glfwGetTime();

    // Calculate mouse move and button
    double mouse_move_x = get_mouse_x() - get_last_mouse_x();
    double mouse_move_y = get_mouse_y() - get_last_mouse_y();
    set_left_mouse_button_state(0);
    set_mouse_move_x(mouse_move_x);
    set_mouse_move_y(mouse_move_y);
    set_right_mouse_button_state(0);
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        set_left_mouse_button_state(1);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        set_right_mouse_button_state(1);

    // Update the keys
    for (std::map<std::string, unsigned short>::iterator it = get_keys_state()->begin(); it != get_keys_state()->end(); it++)
    {
        it->second = 0; // Reset keys
    }

    for (std::map<std::string, unsigned int>::iterator it = keys.begin(); it != keys.end(); it++)
    {
        if (glfwGetKey(window, it->second) == GLFW_PRESS)
            (*get_keys_state())[it->first] = 1;
    }

    // Update key frame
    unsigned short limit = 65000;
    for (std::map<std::string, unsigned short>::iterator it = get_keys_state()->begin(); it != get_keys_state()->end(); it++)
    {
        unsigned short state = it->second;
        if (state != 0)
        {
            if ((*get_keys_state_frame())[it->first] != state && (*get_keys_state_frame())[it->first] != limit)
            {
                (*get_keys_state_frame())[it->first] = state;
            }
            else
            {
                (*get_keys_state_frame())[it->first] = limit;
            }
        }
        else
        {
            (*get_keys_state_frame())[it->first] = 0; // Reset keys
        }
    }

    // Update last mouse pos for future mouse pos calcul
    set_last_mouse_x(get_mouse_x());
    set_last_mouse_y(get_mouse_y());
}

// Game destructor
Game::~Game()
{
    std::map<std::string, Scene*> *scenes = get_scenes();
    for (std::map<std::string, Scene*>::iterator it = scenes->begin(); it != scenes->end(); it++)
    {
        delete it->second; // Destroy each scenes
        it->second = 0;
    }

    std::map<std::string, HUD*>* huds = get_huds();
    for (std::map<std::string, HUD*>::iterator it = huds->begin(); it != huds->end(); it++)
    {
        delete it->second; // Destroy each HUD_Object
        it->second = 0;
    }
}
