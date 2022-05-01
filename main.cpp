#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "scene.hpp"

const int kOpenGLVersionMejor = 3;
const int kOpenGLVersionMinor = 3;

int window_width = 800;
int window_height = 600;

const glm::vec4 kBackgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
const float kPointSize = 8.0f;

std::unique_ptr<Scene> scene;

// Callback functions
void glfwErrorCallBack(int error, const char* description) {
    std::cerr << "GLFW Error" << error << ": " << description << std::endl;
}

void glfwCursorPosCallBack(GLFWwindow* window, double x, double y) {

}

void glfwMouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {

}

void glfwKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_ESCAPE:
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            default:
                break;    
        }
    }
    // TODO: Move camera by mouse
    if(action == GLFW_REPEAT) {
        switch(key) {
            case GLFW_KEY_W:
                scene->MoveCamera(CameraMovement::UP);
                break;
            case GLFW_KEY_S:
                scene->MoveCamera(CameraMovement::DOWN);
                break;
            case GLFW_KEY_A:
                scene->MoveCamera(CameraMovement::LEFT);
                break;
            case GLFW_KEY_D:
                scene->MoveCamera(CameraMovement::RIGHT);
                break;
            default:
                break;
        }
    }
}

void glfwFramebufferSizeCallBack(GLFWwindow* window, int width, int height) {
    window_width = width;
    window_height = height;
    // TODO: set aspect ratio to camera
    glViewport(0, 0, width, height);
}

int main(int argc, char* argv[]) {
    // Initialize GLFW
    if(!glfwInit()) exit(1);
    glfwSetErrorCallback(glfwErrorCallBack);

    // Set function to be executed on exit
    atexit(glfwTerminate);

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kOpenGLVersionMejor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kOpenGLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_TRUE);
#endif

    // Create a window
    GLFWwindow *window = glfwCreateWindow(window_width, window_height, "SPH Based Shallow Water Simulation", NULL, NULL);
    if(window == NULL) {
        std::cerr << "Falied to create GLFW window" << std::endl;
        exit(1);
    }
    glfwMakeContextCurrent(window);

    // Register callback functions
    glfwSetCursorPosCallback(window, glfwCursorPosCallBack);
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallBack);
    glfwSetKeyCallback(window, glfwKeyCallBack);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallBack);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        std::cerr << "Falied to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }

    // Settings
    glfwSwapInterval(0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_POINT_SMOOTH);

    glClearColor(kBackgroundColor.r, kBackgroundColor.g, kBackgroundColor.b, kBackgroundColor.a);
    glPointSize(kPointSize);

    // Create a scene
    scene = std::make_unique<Scene>(window_width, window_height);

    // Set timer
    float current_time = 0.0f, last_time = 0.0f, elapsed_time = 0.0f;
    glfwSetTime(0.0);

    // Main loop
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->Display();

        // Timer
        current_time = glfwGetTime();
        elapsed_time = current_time - last_time;
        if(elapsed_time >= scene->GetDeltaTime()) {
            scene->Update();
            last_time = current_time;
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Terminate program
    glfwDestroyWindow(window);
    exit(0);
}