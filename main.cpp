#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<iostream>

#include "scene.hpp"

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
}

void glfwFramebufferSizeCallBack(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main(int argc, char* argv[]) {
    // Initialize GLFW
    if(!glfwInit()) return 1;
    glfwSetErrorCallback(glfwErrorCallBack);

    // Set function to be executed on exit
    atexit(glfwTerminate);

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_TRUE);
#endif

    // Create a window
    GLFWwindow *window = glfwCreateWindow(800, 600, "SPH Based Shallow Water Simulation", NULL, NULL);
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

    // Disable V-Sync
    glfwSwapInterval(0);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable face culling
    glEnable(GL_CULL_FACE);

    // Background color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Set timer
    float current_time = 0.0f, last_time = 0.0f, elapsed_time = 0.0f;
    glfwSetTime(0.0);

    // Main loop
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Timer
        current_time = glfwGetTime();
        elapsed_time = current_time - last_time;
        last_time = current_time;

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Terminate program
    glfwDestroyWindow(window);
    exit(0);
}