#include "model.h"
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

namespace fs = std::filesystem;


Model::Model(glm::vec3 pos, const std::string &path)
    : m_dir(fs::path(path).parent_path())
{
    Assimp::Importer im;
    const aiScene *scene = im.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        fprintf(stderr, "[model_alloc] Assimp error when loading model.\n");
        exit(EXIT_FAILURE);
    }

    process_node(scene->mRootNode, scene);

    m_translation = glm::mat4(1.f);
    m_rotation = glm::mat4(1.f);

    move(pos);
}


Model::~Model()
{
}


void Model::render(RenderInfo *ri)
{
    glm::mat4 model = m_translation * m_rotation;

    for (size_t i = 0; i < m_meshes.size(); ++i)
        m_meshes[i]->render(ri, model);
}


void Model::move(glm::vec3 dir)
{
    m_translation = glm::translate(m_translation, dir);
    m_pos += dir;
}


void Model::rotate(float rad, glm::vec3 axis)
{
    m_rotation = glm::rotate(m_rotation, rad, axis);
}


void Model::process_node(aiNode *node, const aiScene *sc)
{
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        struct aiMesh *mesh = sc->mMeshes[node->mMeshes[i]];
        m_meshes.emplace_back(process_mesh(mesh, sc));
    }

    for (size_t i = 0; i < node->mNumChildren; ++i)
    {
        process_node(node->mChildren[i], sc);
    }
}


std::unique_ptr<Mesh> Model::process_mesh(aiMesh *mesh, const aiScene *sc)
{
    std::vector<Vertex> verts;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;

    // vertices
    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex v;
        v.pos.x = mesh->mVertices[i].x;
        v.pos.y = mesh->mVertices[i].y;
        v.pos.z = mesh->mVertices[i].z;

        v.norm.x = mesh->mNormals[i].x;
        v.norm.y = mesh->mNormals[i].y;
        v.norm.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0])
        {
            v.tex_coords.x = mesh->mTextureCoords[0][i].x;
            v.tex_coords.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            v.tex_coords = glm::vec2(0.f, 0.f);
        }

        verts.emplace_back(v);
    }

    // indices
    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        struct aiFace face = mesh->mFaces[i];

        for (size_t j = 0; j < face.mNumIndices; ++j)
            indices.emplace_back(face.mIndices[j]);
    }

    // material
    struct aiMaterial *mat = sc->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture*> diffuse = load_mat_textures(mat, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
    std::vector<Texture*> specular = load_mat_textures(mat, aiTextureType_SPECULAR, TextureType::SPECULAR);

    for (size_t i = 0; i < diffuse.size(); ++i)
        textures.emplace_back(diffuse[i]);

    for (size_t i = 0; i < specular.size(); ++i)
        textures.emplace_back(specular[i]);

    return std::make_unique<Mesh>(verts, indices, textures);
}


std::vector<Texture*> Model::load_mat_textures(aiMaterial *mat, aiTextureType type, TextureType ttype)
{
    size_t count = mat->GetTextureCount(type);
    std::vector<Texture*> textures;

    for (size_t i = 0; i < count; ++i)
    {
        aiString s;
        mat->GetTexture(type, i, &s);

        std::string path = m_dir + '/' + s.data;
        bool skip = false;

        for (size_t i = 0; i < m_textures.size(); ++i)
        {
            if (m_textures[i]->path() == path)
            {
                textures.emplace_back(m_textures[i].get());
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            m_textures.emplace_back(std::make_unique<Texture>(path, ttype));
            textures.emplace_back(m_textures[m_textures.size() - 1].get());
        }
    }

    return textures;
}

