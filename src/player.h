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

    void update();
    void render(RenderInfo &ri);

    void move(glm::vec3 dir);
    void rotate(glm::vec3 rot);

    void update_weapon();

    void scope(bool b) { m_scoped = b; }

    void set_vel(glm::vec3 vel) { m_vel = vel; }

    const Camera &cam() const { return m_cam; }

private:
    Camera m_cam;
    Weapon m_gun;
    bool m_scoped;

    glm::vec3 m_vel;
};

