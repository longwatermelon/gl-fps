#pragma once
#include "camera.h"
#include "model.h"
#include "weapon.h"
#include <glm/glm.hpp>

class Player
{
public:
    Player(glm::vec3 pos, glm::vec3 rot);
    ~Player();

    void render(RenderInfo &ri);

    void move(glm::vec3 dir);
    void rotate(glm::vec3 rot);

    void update_weapon();

    const Camera &cam() const { return m_cam; }

private:
    Camera m_cam;
    Weapon m_gun;
};

