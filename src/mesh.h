#pragma once
#include "texture.h"
#include "render.h"
#include <vector>
#include <memory>
#include <array>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 pos, norm;
    glm::vec2 tex_coords;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex> &verts, const std::vector<unsigned int> &indices, const std::vector<Texture*> &textures);
    Mesh(Mesh&&) = default;
    ~Mesh();

    void render(RenderInfo &ri, glm::mat4 model) const;

    float shortest_dist(glm::vec3 p, glm::vec3 *norm) const;
    float shortest_dist_tri(std::array<glm::vec3, 3> pts, glm::vec3 p, glm::vec3 *norm) const;
    bool ray_intersect(glm::vec3 orig, glm::vec3 dir, float *t) const;
    bool ray_intersect_tri(glm::vec3 orig, glm::vec3 dir, std::array<glm::vec3, 3> pts, float *t) const;

    void update_pos(glm::vec3 pos) { m_pos = pos; }
    void update_rot(glm::vec3 rot) { m_rot = rot; }

private:
    glm::vec3 m_pos, m_rot;

    std::vector<Vertex> m_verts;
    std::vector<unsigned int> m_indices;
    std::vector<Texture*> m_textures;

    unsigned int m_vao, m_vb, m_ib;
};

