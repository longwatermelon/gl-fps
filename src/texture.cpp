#include "texture.h"
#include <iostream>
#include <glad/glad.h>
#include <stb/stb_image.h>


Texture::Texture(const std::string &path, TextureType type)
    : m_type(type), m_path(path)
{
    glGenTextures(1, &m_id);

    int w, h, nchannels;
    unsigned char *data = stbi_load(path.c_str(), &w, &h, &nchannels, 0);

    if (data)
    {
        GLenum format;

        switch (nchannels)
        {
        case 1: format = GL_RED; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default:
            std::cerr << "[tex_alloc] Expected 1, 3 or 4 channels in image '" << m_path << "' but " << nchannels << " were given.\n";
            exit(EXIT_FAILURE);
        }

        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cerr << "[tex_alloc] Failed to load file '" << path << "'.\n";
        exit(EXIT_FAILURE);
    }
}


Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}


void Texture::bind(unsigned int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

