#pragma once

#include <iostream>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "shader.hpp"
#include "simulator.hpp"

enum CameraMovement {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Scene {
public:
    Scene(int window_width, int window_height);
    ~Scene();

    // getter
    float GetDeltaTime() const;

    // setter
    void SetAspectRatio(int window_width, int window_height);

    void Display();
    void Update();
    void MoveCamera(CameraMovement movement);

private:
    void LoadConfiguration(int window_width, int window_height);
    void GenerateBuffers();
    void UpdateBuffers();

    // Camera
    std::unique_ptr<Camera> camera_;
    float distance_;
    float theta_;
    float phi_;

    // Light
    glm::vec3 light_pos_;
    glm::vec3 light_color_;

    // Shader 
    std::unique_ptr<Shader> particle_shader_;

    // Simulator
    std::unique_ptr<SPHSWE> simulator_;

    // Buffers
    GLuint particle_vao_;
    GLuint particle_vbo_;
};