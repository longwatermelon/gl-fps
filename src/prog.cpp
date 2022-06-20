#include "prog.h"
#include "light.h"
#include "model.h"
#include <vector>
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stb/stb_image.h>


Prog::Prog(GLFWwindow *w)
    : m_win(w), m_player(glm::vec3(-30.f, 70.f, 0.f), glm::vec3(0.f, 0.f, 0.f))
{
    m_ri.add_shader("basic");
    m_ri.add_shader("white");
    m_ri.add_shader("skybox");

    m_ri.view = glm::mat4(1.f);
    m_ri.proj = glm::perspective(glm::radians(45.f), 800.f / 600.f, .01f, 1000.f);

    m_ri.cam = &m_player.cam();

    m_solids.emplace_back(Model(glm::vec3(0.f, 0.f, -40.f), "res/ground/untitled.obj"));
    m_enemies.emplace_back(Enemy(glm::vec3(0.f, 50.f, 0.f), "res/enemy/enemy.obj"));

    m_player_last_shot = 0.f;
}


Prog::~Prog()
{
    m_ri.clear_shaders();
}


void Prog::mainloop()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glfwSetCursorPos(m_win, 400.f, 300.f);
    glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    double prev_mx, prev_my;
    glfwGetCursorPos(m_win, &prev_mx, &prev_my);

    while (!glfwWindowShouldClose(m_win))
    {
        events();

        double mx, my;
        glfwGetCursorPos(m_win, &mx, &my);
        m_player.rotate(glm::vec3(0.f, -(mx - prev_mx) / 100.f, -(my - prev_my) / 100.f));

        prev_mx = mx;
        prev_my = my;

        m_player.update(m_solids);

        for (size_t i = 0; i < m_enemies.size(); ++i)
        {
            m_enemies[i].update();

            if (m_enemies[i].health() < 0)
                m_enemies.erase(m_enemies.begin() + i--);
        }

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(m_ri.shaders["skybox"]);
        m_skybox.render(m_ri);

        glUseProgram(m_ri.shaders["basic"]);
        m_ri.view = glm::lookAt(m_player.cam().pos(), m_player.cam().pos() + m_player.cam().front(), m_player.cam().up());
        m_player.set_props(m_ri.shaders["basic"]);

        for (auto &s : m_solids)
            s.render(m_ri);

        for (auto &e : m_enemies)
            e.render(m_ri);

        glClear(GL_DEPTH_BUFFER_BIT);
        m_player.render(m_ri);

        if (m_player.scoped())
            draw_crosshair();

        glfwSwapBuffers(m_win);
        glfwPollEvents();
    }
}


void Prog::events()
{
    float move = .2f;
    float rot = 2.f;

    if (m_player.scoped())
        move *= .5f;

    glm::quat yaw(glm::vec3(0.f, m_player.cam().rot().y, 0.f));
    glm::vec3 front = yaw * glm::vec3(1.f, 0.f, 0.f) * move;
    glm::vec3 right = yaw * glm::vec3(0.f, 0.f, 1.f) * move;

    glm::vec3 vec(0.f, 0.f, 0.f);

    if (glfwGetKey(m_win, GLFW_KEY_W) == GLFW_PRESS) vec += front;
    if (glfwGetKey(m_win, GLFW_KEY_S) == GLFW_PRESS) vec -= front;
    if (glfwGetKey(m_win, GLFW_KEY_A) == GLFW_PRESS) vec -= right;
    if (glfwGetKey(m_win, GLFW_KEY_D) == GLFW_PRESS) vec += right;

    if (glfwGetKey(m_win, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (m_player.vel().y == 0.f)
            m_player.set_vely(.3f);
    }

    if (glfwGetKey(m_win, GLFW_KEY_LEFT) == GLFW_PRESS) m_player.rotate(glm::vec3(-rot, 0.f, 0.f));
    if (glfwGetKey(m_win, GLFW_KEY_RIGHT) == GLFW_PRESS) m_player.rotate(glm::vec3(rot, 0.f, 0.f));
    if (glfwGetKey(m_win, GLFW_KEY_UP) == GLFW_PRESS) m_player.rotate(glm::vec3(0.f, rot, 0.f));
    if (glfwGetKey(m_win, GLFW_KEY_DOWN) == GLFW_PRESS) m_player.rotate(glm::vec3(0.f, -rot, 0.f));

    if (glfwGetMouseButton(m_win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (glfwGetTime() - m_player_last_shot > .5f)
        {
            m_player_last_shot = glfwGetTime();
            m_player.shoot_effects();

            Enemy *e = m_player.shoot(m_enemies);
            if (e)
                e->damage(1);
        }
    }

    m_player.scope(glfwGetMouseButton(m_win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

    m_player.set_velx(vec.x);
    m_player.set_velz(vec.z);
}


void Prog::draw_crosshair()
{
    float verts[] = {
        400.f, 305.f,
        400.f, 295.f,
        395.f, 300.f,
        405.f, 300.f
    };

    for (size_t i = 0; i < sizeof(verts) / sizeof(float); i += 2)
    {
        verts[i] = (verts[i] / 400.f - 1.f) * 1.5f;
        verts[i + 1] = (verts[i + 1] / 300.f - 1.f) * 1.5f;
    }

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vb;
    glGenBuffers(1, &vb);

    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glUseProgram(m_ri.shaders["white"]);
    glDrawArrays(GL_LINES, 0, 4);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vb);
}

