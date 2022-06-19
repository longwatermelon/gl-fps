#pragma once
#include "model.h"
#include "render.h"

class Enemy
{
public:
    Enemy(glm::vec3 pos, const std::string &path);
    Enemy(Enemy&&) = default;
    ~Enemy();

    void update();
    void render(RenderInfo &ri);

    void move(glm::vec3 dir);
    void rotate(glm::vec3 rot);

private:
    Model m_model;
};

