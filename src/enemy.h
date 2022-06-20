#pragma once
#include "model.h"
#include "render.h"

class Enemy
{
public:
    Enemy(glm::vec3 pos, const std::string &path);
    Enemy(Enemy&&) = default;
    Enemy& operator=(Enemy&&) = default;
    ~Enemy();

    void update();
    void render(RenderInfo &ri) const;

    void move(glm::vec3 dir);
    void move_towards_player(glm::vec3 cam);

    void rotate(glm::vec3 rot);

    bool ray_intersect(glm::vec3 orig, glm::vec3 dir, float *t) const;
    void damage(int damage);

    int health() const { return m_health; }
    const Model &model() const { return m_model; }

    bool should_remove() const { return m_remove; }

private:
    Model m_model;
    int m_health = 5;

    glm::vec3 m_death_vector;
    float m_death_time = 0.f;

    bool m_remove = false;
};

