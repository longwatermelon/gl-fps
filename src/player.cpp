#include "player.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>


Player::Player(glm::vec3 pos, glm::vec3 rot)
    : m_cam(pos, rot), m_gun("res/gun/Gun.obj", pos, rot, 5.f), m_scoped(false), m_vel(0.f, 0.f, 0.f),
      m_light(pos, Phong(
        glm::vec3(.2f, .2f, .2f),
        glm::vec3(.5f, .5f, .5f),
        glm::vec3(1.f, 1.f, 1.f)
      ), Attenuation(1.f, .009f, .0032f))
{
}


Player::~Player()
{
}


void Player::update(const std::vector<Model> &solids)
{
    m_vel.y -= .01f;

    move(m_vel, solids);
    update_weapon();
}


void Player::render(RenderInfo &ri)
{
    m_gun.render(ri);
}


void Player::move(glm::vec3 dir)
{
    m_cam.move(dir);
    m_gun.move(dir);
    m_light.move(m_cam.pos());
}


void Player::move(glm::vec3 dir, const std::vector<Model> &solids)
{
    glm::vec3 x(dir.x, 0.f, 0.f);
    glm::vec3 y(0.f, dir.y, 0.f);
    glm::vec3 z(0.f, 0.f, dir.z);

    if (!move_dir(x, solids, .5f)) m_vel.x = 0.f;
    if (!move_dir(y, solids, 3.f)) m_vel.y = 0.f;
    if (!move_dir(z, solids, .5f)) m_vel.z = 0.f;
}


bool Player::move_dir(glm::vec3 dir, const std::vector<Model> &solids, float min_dist)
{
    float t;

    if (check_dir(dir, solids, min_dist, &t))
    {
        move(dir);
        return true;
    }
    else
    {
        float dist;
        check_dir(glm::vec3(0.f, 0.f, 0.f), solids, min_dist, &dist);

        float diff = dist - min_dist;
        move(glm::normalize(dir) * diff);
        return false;
    }
}


bool Player::check_dir(glm::vec3 dir, const std::vector<Model> &solids, float min_dist, float *t)
{
    *t = INFINITY;
    glm::vec3 moved = m_cam.pos() + dir;

    for (auto &s : solids)
    {
        float dist = s.shortest_dist(moved);

        if (dist < *t)
            *t = dist;
    }

    return *t > min_dist;
}


void Player::rotate(glm::vec3 rot)
{
    m_cam.rotate(rot);
}


void Player::update_weapon()
{
    m_gun.set_target_pos(m_cam.pos() + m_cam.front() +
        (m_scoped ? glm::vec3(0.f, 0.f, 0.f) : m_cam.right() / 2.f) +
        (-m_cam.up() / 3.f));
    m_gun.set_target_rot(m_cam.rot());
    m_gun.update();
}


void Player::set_props(unsigned int shader)
{
    m_cam.set_props(shader);
    m_light.set_props(shader, 0);
}

