#pragma once
#include "model.h"
#include "render.h"
#include "util.h"
#include <glm/glm.hpp>

class Weapon
{
public:
    Weapon(const std::string &path, glm::vec3 pos, glm::vec3 rot, float divisor);
    ~Weapon();

    void render(RenderInfo &ri);

    void update();

    void set_target_pos(glm::vec3 pos);
    void set_target_rot(glm::vec3 rot);

private:
    Model m_model;
    glm::vec3 m_target_pos, m_target_rot;

    float m_divisor;
};

