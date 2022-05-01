#include "scene.hpp"

Scene::Scene(int window_width, int window_height) {
    LoadConfiguration(window_width, window_height);
    GenerateBuffers();
}

Scene::~Scene() {
    glDeleteBuffers(1, &particle_vbo_);
    glDeleteVertexArrays(1, &particle_vao_);
}

// getter
float Scene::GetDeltaTime() const {
    return simulator_->GetDeltaTime();
}

// setter
void Scene::SetAspectRatio(int window_width, int window_height) {
    float aspect_ratio = (float)window_width / (float)window_height;
    camera_->SetAspectRatio(aspect_ratio);
}

void Scene::Display() {
    glm::mat4 view = camera_->GenViewMatrix();      
    glm::mat4 projection = camera_->GenProjectionMatrix();
    
    // Draw particles
    particle_shader_->Use();
    glm::mat4 model = glm::mat4(1.0f);
    particle_shader_->SetMat4("model", model);
    particle_shader_->SetMat4("view", view);
    particle_shader_->SetMat4("projection", projection);
    glBindVertexArray(particle_vao_);
    glDrawArrays(GL_POINTS, 0, simulator_->GetNumParticles());
    glBindVertexArray(0);
}

void Scene::Update() {
    simulator_->Evolve();
    UpdateBuffers();
}

void Scene::MoveCamera(CameraMovement movement) {
    const float intensity = 1.0f;

    switch(movement)
    {
        case UP: 
            theta_ += intensity;
            break;
        case DOWN: 
            theta_ -= intensity;
            break;
        case LEFT:
            phi_ -= intensity;
            break;
        case RIGHT:
            phi_ += intensity;
            break;
    }

    // Clamp
    if(theta_ >  89.0f) theta_ =  89.0f;
    if(theta_ < -89.0f) theta_ = -89.0f;

    // Convert radian
    float theta_rad = glm::radians(theta_);
    float phi_rad = glm::radians(phi_);

    camera_->SetPosition(distance_ * glm::vec3(cosf(theta_rad)*cosf(phi_rad), sinf(theta_rad), -cosf(theta_rad)*sinf(phi_rad)));
}

// TODO: load from configuration file
void Scene::LoadConfiguration(int window_width, int window_height) {
    // Camera
    distance_ = 3.0f;
    theta_ = 0.0f;
    phi_ = -90.0f;
    float theta_rad = glm::radians(theta_);
    float phi_rad = glm::radians(phi_);
    glm::vec3 camera_pos = distance_ * glm::vec3(cosf(theta_rad)*cosf(phi_rad), sinf(theta_rad), -cosf(theta_rad)*sinf(phi_rad));
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    float fov = 45.0f;
    float aspect_ratio = (float)window_width/(float)window_height;
    float clliping_near = 0.1f;
    float clliping_far = 100.0f;
    camera_ = std::make_unique<Camera>(camera_pos, target, world_up, fov, aspect_ratio, clliping_near, clliping_far);

    // Light
    light_pos_ = glm::vec3(1.0f, 1.0f, 1.0f);
    light_color_ = glm::vec3(1.0f, 1.0f, 1.0f);

    // Shader
    particle_shader_ = std::make_unique<Shader>("../shader/vertex.glsl", "../shader/fragment.glsl");

    // Simulator
    float scale = 4.0;
    float delta_time = 0.002;
    simulator_ = std::make_unique<SPHSWE>(scale, delta_time);
}

void Scene::GenerateBuffers() {
    // Must be deleted in destructor
    Particle* data = simulator_->GetData();
    glGenVertexArrays(1, &particle_vao_);
    glGenBuffers(1, &particle_vbo_);
    glBindVertexArray(particle_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, particle_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle)*simulator_->GetNumParticles(), data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
    glEnableVertexAttribArray(0);
}

void Scene::UpdateBuffers() {
    // glBindBuffer(GL_ARRAY_BUFFER, particle_vbo_);
    // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle)*simulator_->GetNumParticles(), simulator_->GetData());
}
