#include "camera.h"
#include "shader.h"
#include <string>
#include <iostream>
#include <glad/glad.h>
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
    update_vectors();
}


void Camera::update_vectors()
{
    glm::vec3 front(1.f, 0.f, 0.f);
    /* glm::quat qh(glm::vec3(0.f, m_rot.y, 0.f)); */
    /* glm::quat qv(glm::vec3(0.f, 0.f, m_rot.z)); */

    /* front = front * qh; */
    /* front = qv * front; */
    glm::quat quat(m_rot);
    quat = glm::normalize(quat);
    front = quat * front;

    std::cout << glm::to_string(m_rot) << " | " << glm::to_string(quat) << "\n";
    /* glm::vec3 front = { */
    /*     cosf(m_rot.y) * cosf(m_rot.z), */
    /*     sinf(m_rot.z), */
    /*     sinf(m_rot.y) * cosf(m_rot.z) */
    /* }; */

    glm::vec3 right = glm::cross(front, quat * glm::vec3(0.f, 1.f, 0.f));
    glm::vec3 up = glm::cross(right, front);

    /* std::cout << glm::to_string(front) << "\n"; */

    m_front = glm::normalize(front);
    m_right = glm::normalize(right);
    m_up = glm::normalize(up);
}


void Camera::set_props(unsigned int shader) const
{
    shader_vec3(shader, std::string("viewPos"), m_pos);
}

