#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 world_up, float fov, float aspect_ratio, float clliping_near, float clliping_far);

    glm::mat4 GenViewMatrix();
    glm::mat4 GenProjectionMatrix();

    void SetPosition(glm::vec3 position);

private:
    // Calculate vectors
    void Update();

    glm::vec3 position_;
    glm::vec3 target_;
    glm::vec3 world_up_;
    glm::vec3 front_;
    glm::vec3 right_;
    glm::vec3 up_;
    float fov_;
    float aspect_ratio_;
    float clliping_near_;
    float clliping_far_;
};