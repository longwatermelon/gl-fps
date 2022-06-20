#include "enemy.h"


Enemy::Enemy(glm::vec3 pos, const std::string &path)
    : m_model(pos, path)
{
}


Enemy::~Enemy()
{
}


void Enemy::update()
{
    m_model.rotate(glm::vec3(0.f, 0.04f, .08f));
}


void Enemy::render(RenderInfo &ri) const
{
    m_model.render(ri);
}


void Enemy::move(glm::vec3 dir)
{
    m_model.move(dir);
}


void Enemy::rotate(glm::vec3 rot)
{
    m_model.rotate(rot);
}


bool Enemy::ray_intersect(glm::vec3 orig, glm::vec3 dir, float *t) const
{
    return m_model.ray_intersect(orig, dir, t);
}

