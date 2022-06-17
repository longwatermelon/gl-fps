#pragma once
#include "shader.h"
#include <string>
#include <limits>

enum class TextureType
{
    DIFFUSE,
    SPECULAR
};

class Texture
{
public:
    Texture(const std::string &path, TextureType type);
    ~Texture();

    void bind(unsigned int slot);

    TextureType type() const { return m_type; }
    std::string path() const { return m_path; }

    unsigned int id() const { return m_id; }

private:
    TextureType m_type;
    unsigned int m_id;

    std::string m_path;
};

