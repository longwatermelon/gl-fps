#pragma once

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

struct RenderInfo
{
    std::map<std::string, unsigned int> shaders;
    glm::mat4 view, proj;

    void add_shader(const std::string &name);
    void clear_shaders();
};

