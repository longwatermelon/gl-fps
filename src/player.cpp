#include "player.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>


Player::Player(glm::vec3 pos, glm::vec3 rot)
    : m_cam(pos, rot), m_gun("res/gun/Gun.obj", pos, rot, 5.f)
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
    m_cam.rotate(rot);
}


void Player::update_weapon()
{
    m_gun.set_target_pos(m_cam.pos() + m_cam.front() + (m_cam.right() / 2.f) + (-m_cam.up() / 3.f));
    m_gun.set_target_rot(m_cam.rot());
    m_gun.update();
}

