#include "camera.h"
#include "shader.h"
#include <string>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


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
    update_vectors();
}


void Camera::update_vectors()
{
    glm::vec3 front = {
        cosf(m_rot.x) * cosf(m_rot.y),
        sinf(m_rot.y),
        sinf(m_rot.x) * cosf(m_rot.y)
    };

    glm::vec3 right = glm::cross(front, glm::vec3(0.f, 1.f, 0.f));
    glm::vec3 up = glm::cross(right, front);

    m_front = glm::normalize(front);
    m_right = glm::normalize(right);
    m_up = glm::normalize(up);
}


void Camera::set_props(unsigned int shader)
{
    shader_vec3(shader, std::string("viewPos"), m_pos);
}

