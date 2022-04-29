#pragma once

#include <iostream>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "shader.hpp"

enum CameraMovement {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Scene {
public:
    Scene();
    ~Scene();

    // Draw scene
    void Display();

    // Control camera
    void MoveCamera(CameraMovement movement);

private:
    // tmp test
    GLuint vao_;
    GLuint vbo_;
    GLuint ibo_;

    // Camera
    std::unique_ptr<Camera> camera_;
    float distance_;
    float theta_;
    float phi_;

    // Shader 
    std::unique_ptr<Shader> shader_;
};