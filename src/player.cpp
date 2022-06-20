#include "player.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>


Player::Player(glm::vec3 pos, glm::vec3 rot)
    : m_cam(pos, rot), m_gun("res/gun/Gun.obj", pos, rot, 5.f), m_scoped(false), m_vel(0.f, 0.f, 0.f),
      m_light(pos, Phong(
        glm::vec3(.2f, .2f, .2f),
        glm::vec3(.5f, .5f, .5f),
        glm::vec3(1.f, 1.f, 1.f)
      ), Attenuation(1.f, .00009f, .000032f)),
      m_gun_light(pos, Phong(
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 0.f)
      ), Attenuation(1.f, .009f, .0032f))
{
    m_gun_light.make_spotlight(m_cam.front(), cosf(glm::radians(30.f)), cosf(glm::radians(50.f)));
}


Player::~Player()
{
}


void Player::update(const std::vector<Model> &solids)
{
    m_gun_light.move(m_cam.pos() + m_cam.front() * 1.5f);
    m_gun_light.set_spotlight_dir(m_cam.front());

    m_vel.y -= .01f;

    move(m_vel, solids);
    update_weapon();

    Phong phong = m_gun_light.phong();
    glm::vec3 brightness = phong.ambient - phong.ambient / 10.f;
    m_gun_light.set_phong(Phong(brightness, brightness, brightness));

    m_cam.shake();
}


void Player::render(RenderInfo &ri) const
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
    glm::vec3 norm;

    if (check_dir(dir, solids, min_dist, &t, &norm))
    {
        move(dir);
        return true;
    }
    else
    {
        float diff = t - min_dist;

        if (glm::dot(glm::normalize(dir), norm) < 0.f)
            diff *= -1.f;

        move(glm::normalize(dir) * diff);
        return false;
    }
}


bool Player::check_dir(glm::vec3 dir, const std::vector<Model> &solids, float min_dist, float *t, glm::vec3 *norm) const
{
    *t = INFINITY;
    glm::vec3 moved = m_cam.pos() + dir;

    for (auto &s : solids)
    {
        float dist = s.shortest_dist(moved, norm);

        if (dist < *t)
            *t = dist;
    }

    return *t > min_dist;
}


void Player::rotate(glm::vec3 rot)
{
    m_cam.rotate(rot);
}


Enemy *Player::shoot(std::vector<Enemy> &enemies) const
{
    float nearest = INFINITY;
    Enemy *enemy = 0;

    for (auto &e : enemies)
    {
        float dist;

        if (e.ray_intersect(m_cam.pos(), m_cam.front(), &dist) && dist < nearest)
        {
            nearest = dist;
            enemy = &e;
        }
    }

    return enemy;
}


void Player::shoot_effects()
{
    m_gun.set_rot(m_cam.rot() + glm::vec3(0.f, 0.f, .4f));

    glm::vec3 brightness(2.f, 2.f, 2.f);
    m_gun_light.set_phong(Phong(brightness, brightness, brightness));

    m_cam.start_shake();
}


void Player::update_weapon()
{
    m_gun.set_target_pos(m_cam.pos() + m_cam.front() +
        (m_scoped ? glm::vec3(0.f, 0.f, 0.f) : m_cam.right() / 2.f) +
        (-m_cam.up() / 3.f));
    m_gun.set_target_rot(m_cam.rot());
    m_gun.update();
}


bool Player::check_enemies(const std::vector<Enemy> &enemies)
{
    for (auto &e : enemies)
    {
        if (e.health() <= 0)
            continue;

        if (glm::length(e.model().pos() - m_cam.pos()) < 2.f && glfwGetTime() - m_last_hurt > 1.f)
        {
            --m_health;
            m_last_hurt = glfwGetTime();
            return true;
        }
    }

    return false;
}


void Player::set_props(unsigned int shader) const
{
    m_cam.set_props(shader);

    /* m_light.set_props(shader, 0); */
    m_gun_light.set_props(shader, 1);
}

