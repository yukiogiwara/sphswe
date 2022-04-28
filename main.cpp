#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<iostream>

void glfwErrorCallBack(int error, const char* description) {
    std::cerr << "GLFW Error" << error << ": " << description << std::endl;
}

int main(int argc, char* argv[]) {
    // Initializes GLFW
    if(!glfwInit()) return 1;
    glfwSetErrorCallback(glfwErrorCallBack);

    // Sets function to be executed on exit
    atexit(glfwTerminate);

    // Configures GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_TRUE);
#endif

    // Creates a window
    GLFWwindow *window = glfwCreateWindow(800, 600, "SPH Based Shallow Water Simulation", NULL, NULL);
    if(window == NULL) {
        std::cerr << "Falied to create GLFW window" << std::endl;
        exit(1);
    }
    glfwMakeContextCurrent(window);

    // Initializes GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        std::cerr << "Falied to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }

    // Disable V-Sync
    glfwSwapInterval(0);

    // Enables depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enables face culling
    glEnable(GL_CULL_FACE);


    // Main loop
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Terminates program
    glfwDestroyWindow(window);
    exit(0);
}