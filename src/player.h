#pragma once
#include "camera.h"
#include "model.h"
#include "weapon.h"
#include "light.h"
#include <glm/glm.hpp>

class Player
{
public:
    Player(glm::vec3 pos, glm::vec3 rot);
    ~Player();

    void update(const std::vector<Model> &solids);
    void render(RenderInfo &ri) const;

    void move(glm::vec3 dir);
    void move(glm::vec3 dir, const std::vector<Model> &solids);
    bool move_dir(glm::vec3 dir, const std::vector<Model> &solids, float min_dist);
    bool check_dir(glm::vec3 dir, const std::vector<Model> &solids, float min_dist, float *t, glm::vec3 *norm) const;

    void rotate(glm::vec3 rot);

    void update_weapon();

    void set_props(unsigned int shader) const;

    void scope(bool b) { m_scoped = b; }

    void set_velx(float x) { m_vel.x = x; }
    void set_vely(float y) { m_vel.y = y; }
    void set_velz(float z) { m_vel.z = z; }

    glm::vec3 vel() const { return m_vel; }
    const Camera &cam() const { return m_cam; }
    bool scoped() const { return m_scoped; }

private:
    Camera m_cam;
    Weapon m_gun;
    bool m_scoped;

    glm::vec3 m_vel;

    Light m_light;
};

