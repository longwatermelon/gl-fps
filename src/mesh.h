#pragma once
#include "texture.h"
#include "render.h"
#include <vector>
#include <memory>
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

    void render(RenderInfo *ri, glm::mat4 model);

private:
    std::vector<Vertex> m_verts;
    std::vector<unsigned int> m_indices;
    std::vector<Texture*> m_textures;

    unsigned int m_vao, m_vb, m_ib;
};

