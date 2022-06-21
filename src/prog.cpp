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
    : m_win(w), m_player(glm::vec3(-30.f, 70.f, 0.f), glm::vec3(0.f, 0.f, 0.f)), m_skybox("res/skybox_water/"),
      m_sun(glm::vec3(-5040.f, 4120.f, -879.f), Phong(
        glm::vec3(.2f, .2f, .2f),
        glm::vec3(1.5f, 1.5f, 1.5f),
        glm::vec3(2.f, 2.f, 2.f)
      ), Attenuation(1.f, .0000000019f, .0000000006f))
{
    m_ri.add_shader("basic");
    m_ri.add_shader("color");
    m_ri.add_shader("skybox");

    m_ri.proj = glm::perspective(glm::radians(45.f), 800.f / 600.f, .01f, 1000.f);
    m_ri.cam = &m_player.cam();

    m_solids.emplace_back(Model(glm::vec3(0.f, 0.f, -40.f), "res/ground/untitled.obj"));

    m_player_last_shot = 0.f;

    // crosshair
    float verts[] = {
        // verts      colors
        400.f, 305.f, 1.f, 1.f, 1.f,
        400.f, 295.f, 1.f, 1.f, 1.f,
        395.f, 300.f, 1.f, 1.f, 1.f,
        405.f, 300.f, 1.f, 1.f, 1.f
    };

    for (size_t i = 0; i < sizeof(verts) / sizeof(float); i += 5)
    {
        verts[i] = (verts[i] / 400.f - 1.f) * 1.5f;
        verts[i + 1] = (verts[i + 1] / 300.f - 1.f) * 1.5f;
    }

    glGenVertexArrays(1, &m_crosshair_vao);
    glBindVertexArray(m_crosshair_vao);

    glGenBuffers(1, &m_crosshair_vb);

    glBindBuffer(GL_ARRAY_BUFFER, m_crosshair_vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // screen color
    m_scr_verts = {
        -1.f, -1.f,   1.f, 0.f, 0.f, // bot left
         1.f, -1.f,   1.f, 0.f, 0.f, // bot right
         1.f,  1.f,   1.f, 0.f, 0.f, // top right
        -1.f,  1.f,   1.f, 0.f, 0.f, // top left
    };

    unsigned int scr_indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glGenVertexArrays(1, &m_scr_vao);
    glBindVertexArray(m_scr_vao);

    glGenBuffers(1, &m_scr_vb);
    glGenBuffers(1, &m_scr_ib);

    glBindBuffer(GL_ARRAY_BUFFER, m_scr_vb);
    glBufferData(GL_ARRAY_BUFFER, m_scr_verts.size() * sizeof(float), m_scr_verts.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_scr_ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(scr_indices), scr_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}


Prog::~Prog()
{
    glDeleteVertexArrays(1, &m_crosshair_vao);
    glDeleteBuffers(1, &m_crosshair_vb);

    glDeleteVertexArrays(1, &m_scr_vao);
    glDeleteBuffers(1, &m_scr_vb);
    glDeleteBuffers(1, &m_scr_ib);

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

        if (m_player.health() > 0)
        {
            m_player.update(m_solids);

            for (size_t i = 0; i < m_enemies.size(); ++i)
            {
                m_enemies[i].update();
                m_enemies[i].move_towards_player(m_player.cam().pos());

                if (m_enemies[i].should_remove())
                    m_enemies.erase(m_enemies.begin() + i--);
            }

            if (m_player.check_enemies(m_enemies))
                m_scr_opacity = .5f;

            m_scr_opacity -= m_scr_opacity / 10.f;

            if (util::randint(0, 1300) <= 2)
            {
                m_enemies.emplace_back(Enemy(glm::vec3(
                    (float)util::randint(-500, 500) / 10.f,
                    (float)util::randint(-200, 500) / 10.f,
                    (float)util::randint(-500, 500) / 10.f
                ), "res/enemy/enemy.obj"));
            }
        }
        else
        {
        }

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(m_ri.shaders["skybox"]);
        m_skybox.render(m_ri);

        glUseProgram(m_ri.shaders["basic"]);
        m_sun.set_props(m_ri.shaders["basic"], 0);
        m_player.set_props(m_ri.shaders["basic"]);

        for (auto &s : m_solids)
            s.render(m_ri);

        for (auto &e : m_enemies)
            e.render(m_ri);

        glClear(GL_DEPTH_BUFFER_BIT);
        m_player.render(m_ri);

        if (m_player.scoped())
            draw_crosshair();

        draw_damage();

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
    glUseProgram(m_ri.shaders["color"]);
    shader_float(m_ri.shaders["color"], std::string("opacity"), 1.f);

    glBindVertexArray(m_crosshair_vao);
    glDrawArrays(GL_LINES, 0, 4);
}


void Prog::draw_damage()
{
    glBindVertexArray(m_scr_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_scr_ib);

    glUseProgram(m_ri.shaders["color"]);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_float(m_ri.shaders["color"], std::string("opacity"), m_scr_opacity);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

