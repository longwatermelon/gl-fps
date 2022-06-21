#include "mesh.h"
#include "util.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>
#include <glad/glad.h>


Mesh::Mesh(const std::vector<Vertex> &verts, const std::vector<unsigned int> &indices, const std::vector<Texture*> &textures)
    : m_verts(verts), m_indices(indices), m_textures(textures)
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vb);
    glGenBuffers(1, &m_ib);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // verts
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // norms
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
    glEnableVertexAttribArray(1);

    // tex
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(2);
}


Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_vb);
    glDeleteBuffers(1, &m_ib);
    glDeleteVertexArrays(1, &m_vao);
}


void Mesh::render(RenderInfo &ri, glm::mat4 model) const
{
    unsigned int diffuse_n = 1, specular_n = 1;

    for (size_t i = 0; i < m_textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        bool is_diffuse = m_textures[i]->type() == TextureType::DIFFUSE;

        std::string prop = is_diffuse ? "diffuse" : "specular";
        size_t num = is_diffuse ? diffuse_n++ : specular_n++;

        std::string s = "material." + prop + std::to_string(num);
        shader_int(ri.shaders["basic"], s, i);

        glBindTexture(GL_TEXTURE_2D, m_textures[i]->id());
    }

    shader_float(ri.shaders["basic"], std::string("material.shininess"), 50.f);

    shader_mat4(ri.shaders["basic"], std::string("model"), &model[0][0]);
    shader_mat4(ri.shaders["basic"], std::string("view"), &ri.cam->view()[0][0]);
    shader_mat4(ri.shaders["basic"], std::string("projection"), &ri.proj[0][0]);

    shader_vec3(ri.shaders["basic"], std::string("material.color"), m_col);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}


float Mesh::shortest_dist(glm::vec3 p, glm::vec3 *norm) const
{
    float t = INFINITY;

    glm::quat yaw(glm::vec3(0.f, m_rot.y, 0.f));
    glm::quat pitch(glm::vec3(0.f, 0.f, m_rot.z));
    glm::quat quat = glm::normalize(yaw * pitch);

    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        std::array<glm::vec3, 3> pts = {
            quat * m_verts[m_indices[i]].pos,
            quat * m_verts[m_indices[i + 1]].pos,
            quat * m_verts[m_indices[i + 2]].pos
        };

        glm::vec3 n;
        float dist = shortest_dist_tri(pts, p, &n);

        if (dist < t)
        {
            t = dist;
            *norm = n;
        }
    }

    return t;
}


float Mesh::shortest_dist_tri(std::array<glm::vec3, 3> pts, glm::vec3 p, glm::vec3 *norm) const
{
    float t = INFINITY;

    glm::vec3 a = m_pos + pts[0];
    glm::vec3 b = m_pos + pts[1];
    glm::vec3 c = m_pos + pts[2];

    *norm = glm::normalize(glm::cross(c - a, b - a));
    t = std::abs(glm::dot(a - p, *norm));

    std::array<glm::vec3, 3> points = { a, b, c };
    glm::vec3 coefficients = util::barycentric_coeffs(points, p + *norm * t);

    int negatives = 0;
    if (coefficients.x < 0.f) ++negatives;
    if (coefficients.y < 0.f) ++negatives;
    if (coefficients.z < 0.f) ++negatives;

    if (negatives == 0)
    {
        return t;
    }
    else if (negatives == 1)
    {
        if (coefficients.x < 0.f) return util::point_line_shortest_dist(p, points[1], points[2]);
        if (coefficients.y < 0.f) return util::point_line_shortest_dist(p, points[0], points[2]);
        if (coefficients.z < 0.f) return util::point_line_shortest_dist(p, points[0], points[1]);
    }
    else
    {
        if (coefficients.x >= 0.f) return glm::length(points[0] - p);
        if (coefficients.y >= 0.f) return glm::length(points[1] - p);
        if (coefficients.z >= 0.f) return glm::length(points[2] - p);
    }

    return t;
}


bool Mesh::ray_intersect(glm::vec3 orig, glm::vec3 dir, float *t) const
{
    *t = INFINITY;

    glm::quat yaw(glm::vec3(0.f, m_rot.y, 0.f));
    glm::quat pitch(glm::vec3(0.f, 0.f, m_rot.z));
    glm::quat quat = glm::normalize(yaw * pitch);

    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        std::array<glm::vec3, 3> pts = {
            m_pos + quat * m_verts[m_indices[i]].pos,
            m_pos + quat * m_verts[m_indices[i + 1]].pos,
            m_pos + quat * m_verts[m_indices[i + 2]].pos
        };

        float dist;

        if (ray_intersect_tri(orig, dir, pts, &dist))
        {
            if (dist < *t)
                *t = dist;
        }
    }

    return *t != INFINITY;
}


bool Mesh::ray_intersect_tri(glm::vec3 orig, glm::vec3 dir, std::array<glm::vec3, 3> pts, float *t) const
{
    glm::vec2 bary;
    return glm::intersectRayTriangle(orig, dir, pts[0], pts[1], pts[2], bary, *t);
}

