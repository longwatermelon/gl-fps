#pragma once
#include "model.h"
#include "render.h"
#include <string>

class Skybox
{
public:
    Skybox(const std::string &prefix);
    ~Skybox();

    void render(RenderInfo &ri);

private:
    unsigned int m_tex;

    unsigned int m_vao, m_vb;
};

