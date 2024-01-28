#include "Game.h"
#include "model.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "matix/stb_image.h"

double global_mouse_x = 500; // Global variable representing the X pos of the mouse
double global_mouse_y = 500; // Global variable representing the Y pos of the mouse

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

// Game constructor
Game::Game(int a_window_width, int a_window_height): Advanced_Struct(global_mouse_x, global_mouse_y), window_height(a_window_height), window_width(a_window_width)
{
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
    glEnable(GL_DEPTH_TEST);
    stbi_set_flip_vertically_on_load(true);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    load_VAOs();
}

// Add an existing scene to the game
void Game::add_scene(std::string name, Scene* scene)
{
    if (contains_scene(name)) { std::cout << "Matix game : error ! The scene \"" << name << "\" you want to add already exists." << std::endl; return; }
    (*get_scenes())[name] = scene;
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

// Create a scene into the game and return it
Scene* Game::new_scene(std::string name, std::string map_path, Map_Opening_Mode mode)
{
    if (contains_scene(name)) { std::cout << "Matix game : error ! The scene \"" << name << "\" you want to create already exists." << std::endl; return 0; }

    Scene* new_scene = new Scene(this, name, map_path, true, true, mode);
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

// Update one frame of the game
void Game::update()
{
    if (contains_scene(get_current_scene_name()))
    {
        get_current_scene()->update();
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
    std::cout << "FPS : " << 1 / get_delta_time() << std::endl;
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
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        (*get_keys_state())["a"] = 1;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        (*get_keys_state())["z"] = 1;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        (*get_keys_state())["s"] = 1;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        (*get_keys_state())["q"] = 1;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        (*get_keys_state())["d"] = 1;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        (*get_keys_state())["space"] = 1;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        (*get_keys_state())["left shift"] = 1;

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
}
