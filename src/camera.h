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

    void start_shake();
    void shake();

    void set_props(unsigned int shader) const;

    glm::mat4 view() const;

    glm::vec3 pos() const { return m_pos; }
    glm::vec3 rot() const { return m_rot; }

    glm::vec3 front() const { return m_front; }
    glm::vec3 up() const { return m_up; }
    glm::vec3 right() const { return m_right; }

    glm::vec3 shake() const { return m_shake; }

private:
    glm::vec3 m_pos, m_rot;
    glm::vec3 m_front, m_up, m_right;

    float m_shake_begin = 0.f;
    glm::vec3 m_shake = glm::vec3(0.f, 0.f, 0.f);
};

