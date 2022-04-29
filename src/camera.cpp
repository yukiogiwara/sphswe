#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 world_up, float fov, float aspect_ratio, float clliping_near, float clliping_far)
: position_(position), target_(target), world_up_(world_up), fov_(fov), aspect_ratio_(aspect_ratio), clliping_near_(clliping_near), clliping_far_(clliping_far) {
    Update();
}

glm::mat4 Camera::GenViewMatrix() {
    glm::mat4 view = glm::lookAt(position_, target_, world_up_);
    return view;
}

glm::mat4 Camera::GenProjectionMatrix() {
    glm::mat4 projection = glm::perspective(fov_, aspect_ratio_, clliping_near_, clliping_far_);
    return projection;
}

void Camera::SetPosition(glm::vec3 position) {
    position_ = position;
    Update();
}

void Camera::Update() {
    front_ = glm::normalize(target_ - position_);
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_    = glm::normalize(glm::cross(right_, front_));
}