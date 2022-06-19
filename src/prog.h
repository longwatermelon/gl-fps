#pragma once
#include "render.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Prog
{
public:
    Prog(GLFWwindow *w);
    ~Prog();

    void mainloop();
    void events();

    void draw_crosshair();

private:
    GLFWwindow *m_win;
    RenderInfo m_ri;

    Player m_player;

    std::vector<Model> m_solids;
    std::vector<Enemy> m_enemies;

    Texture m_crosshair_tex;
};

