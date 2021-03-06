#include "camera.h"
#include "shader.h"
#include "util.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


Camera::Camera(glm::vec3 pos, glm::vec3 rot)
    : m_pos(pos), m_rot(rot)
{
    update_vectors();
}


Camera::~Camera()
{
}


void Camera::move(glm::vec3 dir)
{
    m_pos += dir;
}


void Camera::rotate(glm::vec3 rot)
{
    m_rot += rot;
    m_rot = util::restrict_cam_angle(m_rot);

    update_vectors();
}


void Camera::update_vectors()
{
    glm::vec3 front(1.f, 0.f, 0.f);

    glm::quat yaw(glm::vec3(0.f, m_rot.y, 0.f));
    glm::quat pitch(glm::vec3(0.f, 0.f, m_rot.z));

    glm::quat quat = glm::normalize(yaw * pitch);
    front = quat * front;

    glm::vec3 right = glm::cross(front, quat * glm::vec3(0.f, 1.f, 0.f));
    glm::vec3 up = glm::cross(right, front);

    m_front = glm::normalize(front);
    m_right = glm::normalize(right);
    m_up = glm::normalize(up);
}


void Camera::start_shake()
{
    m_shake_begin = glfwGetTime();
}


void Camera::shake()
{
    if (glfwGetTime() - m_shake_begin < .1f && m_shake_begin != 0.f)
    {
        m_shake = glm::vec3(
            (float)util::randint(-10, 10) / 1000.f,
            (float)util::randint(0, 10) / 1000.f,
            (float)util::randint(-10, 10) / 1000.f
        );
    }
    else
    {
        m_shake = glm::vec3(0.f, 0.f, 0.f);
    }
}


void Camera::set_props(unsigned int shader) const
{
    shader_vec3(shader, std::string("viewPos"), m_pos);
}


glm::mat4 Camera::view() const
{
    glm::vec3 orig = m_pos + m_shake;
    return glm::lookAt(orig, orig + m_front, m_up);
}

