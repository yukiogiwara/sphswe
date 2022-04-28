#pragma once

#include <GL/glew.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
public:
    unsigned int id_;

    Shader(const char* vertex_path, const char* fragment_path);

    void Use();
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;

private:
};