#include "enemy.h"
#include "util.h"
#include <GLFW/glfw3.h>


Enemy::Enemy(glm::vec3 pos, const std::string &path)
    : m_model(pos, path)
{
    m_death_vector = glm::vec3(
        (float)util::randint(-100, 100) / 200.f,
        (float)util::randint(0, 100) / 100.f,
        (float)util::randint(-100, 100) / 200.f
    );
}


Enemy::~Enemy()
{
}


void Enemy::update()
{
    m_model.rotate(glm::vec3(0.f, 0.04f, .08f));

    if (m_health <= 0)
    {
        m_model.move(m_death_vector);
        m_death_vector.y -= .01f;

        if (glfwGetTime() - m_death_time > 5.f)
            m_remove = true;
    }
}


void Enemy::render(RenderInfo &ri) const
{
    m_model.render(ri);
}


void Enemy::move(glm::vec3 dir)
{
    m_model.move(dir);
}


void Enemy::move_towards_player(glm::vec3 cam)
{
    m_model.move(glm::normalize(cam - m_model.pos()) / 10.f);
}


void Enemy::rotate(glm::vec3 rot)
{
    m_model.rotate(rot);
}


bool Enemy::ray_intersect(glm::vec3 orig, glm::vec3 dir, float *t) const
{
    return m_model.ray_intersect(orig, dir, t);
}


void Enemy::damage(int damage)
{
    if (m_health <= 0)
        return;

    m_health -= damage;

    if (m_health <= 0)
        m_death_time = glfwGetTime();
}

