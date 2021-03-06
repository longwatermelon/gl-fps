#include "weapon.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>


Weapon::Weapon(const std::string &path, glm::vec3 pos, glm::vec3 rot, float divisor)
    : m_model(pos, path), m_target_pos(pos), m_target_rot(rot), m_divisor(divisor)
{
}


Weapon::~Weapon()
{
}


void Weapon::update()
{
    m_model.move((m_target_pos - m_model.pos()) / m_divisor);
    m_model.rotate((m_target_rot - m_model.rot()) / m_divisor);
}


void Weapon::render(RenderInfo &ri) const
{
    m_model.render(ri);
}


void Weapon::move(glm::vec3 dir)
{
    m_model.move(dir);
    m_target_pos = m_model.pos();
}


void Weapon::set_target_pos(glm::vec3 pos)
{
    m_target_pos = pos;
}


void Weapon::set_target_rot(glm::vec3 rot)
{
    m_target_rot = rot;
}


void Weapon::set_pos(glm::vec3 pos)
{
    m_model.move(pos - m_model.pos());
}


void Weapon::set_rot(glm::vec3 rot)
{
    m_model.rotate(rot - m_model.rot());
}

