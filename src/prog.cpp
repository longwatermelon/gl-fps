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
    : m_win(w), m_player(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f))
{
    m_ri.shader = shader_create("shaders/basic_v.glsl", "shaders/basic_f.glsl");
    m_ri.view = glm::mat4(1.f);
    m_ri.proj = glm::perspective(glm::radians(45.f), 800.f / 600.f, .01f, 100.f);

    m_solids.emplace_back(Model(glm::vec3(0.f, 0.f, -15.f), "res/ground/untitled.obj"));
}


Prog::~Prog()
{
    glDeleteShader(m_ri.shader);
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

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_ri.view = glm::lookAt(m_player.cam().pos(), m_player.cam().pos() + m_player.cam().front(), m_player.cam().up());
        m_player.set_props(m_ri.shader);

        for (auto &s : m_solids)
            s.render(m_ri);

        glClear(GL_DEPTH_BUFFER_BIT);
        m_player.render(m_ri);

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

    m_player.scope(glfwGetMouseButton(m_win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

    m_player.set_velx(vec.x);
    m_player.set_velz(vec.z);
}

