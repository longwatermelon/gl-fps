#pragma once

#include "texture.h"
#include <glm/glm.hpp>

struct Phong
{
    Phong(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : ambient(ambient), diffuse(diffuse), specular(specular) {}

    glm::vec3 ambient, diffuse, specular;
};

struct Attenuation
{
    Attenuation(float constant, float linear, float quadratic)
        : constant(constant), linear(linear), quadratic(quadratic) {}

    float constant, linear, quadratic;
};

enum class LightType
{
    POINT = 0,
    SPOTLIGHT = 1
};

class Light
{
public:
    Light(glm::vec3 pos, Phong col, Attenuation att);
    ~Light();

    void move(glm::vec3 dest);
    void spotlight_rotate(glm::vec3 rot);

    void set_props(unsigned int shader, int idx);

    Light &make_spotlight(glm::vec3 dir, float cutoff, float outer_cutoff);

private:
    LightType m_type;

    glm::vec3 m_pos;
    Phong m_col;
    Attenuation m_att;

    glm::vec3 m_spotlight_dir;
    float m_spotlight_cutoff, m_spotlight_outer_cutoff;
};

