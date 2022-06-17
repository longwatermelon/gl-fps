#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
    Camera(glm::vec3 pos, glm::vec3 rot);
    ~Camera();

    void move(glm::vec3 dir);
    void rotate(glm::vec3 rot);
    void update_vectors();

    void set_props(unsigned int shader);

    glm::vec3 pos() const { return m_pos; }
    glm::vec3 rot() const { return m_rot; }

    glm::vec3 front() const { return m_front; }
    glm::vec3 up() const { return m_up; }
    glm::vec3 right() const { return m_right; }

private:
    glm::vec3 m_pos, m_rot;
    glm::vec3 m_front, m_up, m_right;
};

