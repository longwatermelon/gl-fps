#pragma once
#include "mesh.h"
#include <assimp/scene.h>

class Model
{
public:
    Model(glm::vec3 pos, const std::string &path);
    ~Model();

    void render(RenderInfo &ri);

    void move(glm::vec3 dir);
    void rotate(glm::vec3 rot);

    void process_node(aiNode *node, const aiScene *sc);
    std::unique_ptr<Mesh> process_mesh(aiMesh *mesh, const aiScene *sc);

    std::vector<Texture*> load_mat_textures(aiMaterial *mat, aiTextureType type, TextureType ttype);

    glm::vec3 pos() const { return m_pos; }
    glm::vec3 rot() const { return m_rot; }

private:
    glm::vec3 m_pos, m_rot;
    std::string m_dir;

    std::vector<std::unique_ptr<Mesh>> m_meshes;
    std::vector<std::unique_ptr<Texture>> m_textures;

    glm::mat4 m_translation, m_rotation;
};

