#pragma once

#include <glm/gtc/constants.hpp>

// 2D

inline float Poly6(float r, float h) {
    if(r < 0 || h < r) return 0;
    float a = 4.0f / (glm::pi<float>()*powf(h, 8));
    return a * powf(h*h-r*r, 3);
}

inline glm::vec3 GradSpiky(glm::vec3 r, float h) {
    float r_len = glm::length(r);
    if(r_len < 0 || h < r_len) return glm::vec3(0.0f);
    float a = -30.0f / (glm::pi<float>()*powf(h, 5));
    return a * powf(h-r_len, 2) * r / r_len;
}

inline float LaplaceVisc(float r, float h) {
    if(r < 0 || h < r) return 0;
    float a = 20.0f / (3*glm::pi<float>()*powf(h, 5));
    return a * (h-r);
}