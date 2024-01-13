#include "Game.h"
#include "model.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"

double mouse_x = 0;
double mouse_y = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouse_x = xpos;
    mouse_y = ypos;
}

// Game constructor
Game::Game(int a_window_width, int a_window_height): window_height(a_window_height), window_width(a_window_width)
{
    // Configurate base_struct
    Base_Struct *base_struct = new Base_Struct(mouse_x, mouse_y);
    base_struct->get_camera()->set_position(glm::vec3(0.0, 0.0, 0.0));
    base_struct->get_camera()->set_rotation(glm::vec3(0.0, 0.0, 0.0));
    base_struct->set_window_height(window_height);
    base_struct->set_window_width(window_width);

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    advanced_struct = new Advanced_Struct(base_struct);
}

// Add a scene to the game
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
Scene* Game::new_scene(std::string name)
{
    if (contains_scene(name)) { std::cout << "Matix game : error ! The scene \"" << name << "\" you want to create already exists." << std::endl; return 0; }

    Scene* new_scene = new Scene(get_advanced_struct(), name);
    add_scene(name, new_scene);
    return new_scene;
}

// Run the game by doing multiples call to update
void Game::run()
{
    while (!glfwWindowShouldClose(window))
    {
        // Clear OpenGL window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate delta time
        get_base_struct()->set_delta_time(glfwGetTime() - last_frame_time);
        last_frame_time = glfwGetTime();

        // Calculate mouse move
        double mouse_move_x = get_base_struct()->get_mouse_x() - get_base_struct()->get_last_mouse_x();
        double mouse_move_y = get_base_struct()->get_mouse_y() - get_base_struct()->get_last_mouse_y();
        get_base_struct()->set_mouse_move_x(mouse_move_x);
        get_base_struct()->set_mouse_move_y(mouse_move_y);

        float rotate_speed = 45;
        float sensitivity = 30;
        float speed = 5;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            get_base_struct()->get_camera()->move(glm::vec3(speed * get_base_struct()->get_delta_time()) * get_base_struct()->get_camera()->get_forward());
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            get_base_struct()->get_camera()->move(glm::vec3(speed * get_base_struct()->get_delta_time()) * -get_base_struct()->get_camera()->get_forward());
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            get_base_struct()->get_camera()->move(glm::vec3(speed * get_base_struct()->get_delta_time()) * get_base_struct()->get_camera()->get_right());
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            get_base_struct()->get_camera()->move(glm::vec3(speed * get_base_struct()->get_delta_time()) * -get_base_struct()->get_camera()->get_right());
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            get_base_struct()->get_camera()->move(glm::vec3(speed * get_base_struct()->get_delta_time()) * get_base_struct()->get_camera()->get_up());
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            get_base_struct()->get_camera()->move(glm::vec3(speed * get_base_struct()->get_delta_time()) * -get_base_struct()->get_camera()->get_up());

        get_base_struct()->get_camera()->rotate(glm::vec3(0.0, sensitivity * get_base_struct()->get_delta_time() * mouse_move_x, 0.0));
        get_base_struct()->get_camera()->rotate(glm::vec3(-sensitivity * get_base_struct()->get_delta_time() * mouse_move_y, 0.0, 0.0));

        get_base_struct()->get_camera()->soft_reset();

        get_base_struct()->set_last_mouse_x(get_base_struct()->get_mouse_x());
        get_base_struct()->set_last_mouse_y(get_base_struct()->get_mouse_y());

        // Update game
        update();

        // Update OpenGL
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
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
}

// Game destructor
Game::~Game()
{
    std::map<std::string, Scene*> *scenes = get_scenes();
    for (std::map<std::string, Scene*>::iterator it = scenes->begin(); it != scenes->end(); it++)
    {
        delete it->second;
        it->second = 0;
    }
    delete advanced_struct;
    advanced_struct = 0;
}
