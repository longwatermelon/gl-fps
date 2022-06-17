#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

struct RenderInfo
{
    unsigned int shader;

    glm::mat4 view, proj;
};

