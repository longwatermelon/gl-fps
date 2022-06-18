#include "player.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>


Player::Player(glm::vec3 pos, glm::vec3 rot)
    : m_cam(pos, rot), m_gun(pos, "res/gun/Gun.obj")
{
}


Player::~Player()
{
}


void Player::render(RenderInfo &ri)
{
    m_gun.render(ri);
}


void Player::move(glm::vec3 dir)
{
    m_cam.move(dir);
}


void Player::rotate(glm::vec3 rot)
{
    /* rot = glm::vec3(.00f, 0.01f, 0.01f); */

    /* std::cout << glm::to_string(m_cam.rot()) << "\n"; */

    m_cam.rotate(rot);
    m_gun.rotate(rot);
}


void Player::update_weapon()
{
    m_gun.move(m_cam.pos() - m_gun.pos() + (m_cam.front() / 5.f) + (m_cam.right() / 15.f));
    /* m_gun.rotate(m_cam.rot().x, m_cam.rot().y, m_cam.rot().z); */
    /* m_cam.rotate(glm::vec3(.01f, 0.00f, 0.00f)); */
}

