#include "light.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>


Light::Light(glm::vec3 pos, Phong col, Attenuation att)
    : m_type(LightType::POINT), m_pos(pos), m_col(col), m_att(att)
{
}


Light::~Light()
{
}


void Light::move(glm::vec3 dest)
{
    m_pos = dest;
}


void Light::spotlight_rotate(glm::vec3 rot)
{
    m_spotlight_dir = rot;
}


void Light::set_props(unsigned int shader, int idx)
{
    std::string begin = "lights[" + std::to_string(idx) + "].";

    // base parameters
    shader_int(shader, (begin + "type"), (int)m_type);
    shader_vec3(shader, (begin + "position"), m_pos);
    shader_vec3(shader, (begin + "ambient"), m_col.ambient);
    shader_vec3(shader, (begin + "diffuse"), m_col.diffuse);
    shader_vec3(shader, (begin + "specular"), m_col.specular);
    shader_float(shader, (begin + "constant"), m_att.constant);
    shader_float(shader, (begin + "linear"), m_att.linear);
    shader_float(shader, (begin + "quadratic"), m_att.quadratic);

    // spotlight
    if (m_type == LightType::SPOTLIGHT)
    {
        shader_vec3(shader, (begin + "spotlight_dir"), m_spotlight_dir);
        shader_float(shader, (begin + "spotlight_cutoff"), m_spotlight_cutoff);
        shader_float(shader, (begin + "spotlight_outer_cutoff"), m_spotlight_outer_cutoff);
    }
}


Light &Light::make_spotlight(glm::vec3 dir, float cutoff, float outer_cutoff)
{
    m_type = LightType::SPOTLIGHT;

    m_spotlight_dir = dir;
    m_spotlight_cutoff = cutoff;
    m_spotlight_outer_cutoff = outer_cutoff;

    return *this;
}

