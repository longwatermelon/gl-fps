#pragma once
#include "model.h"
#include "render.h"
#include <string>

class Skybox
{
public:
    Skybox();
    ~Skybox();

    void render(RenderInfo &ri);

private:
    unsigned int m_tex;

    unsigned int m_vao, m_vb;
};

