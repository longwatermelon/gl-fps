#include "render.h"
#include "shader.h"
#include <glad/glad.h>


void RenderInfo::add_shader(const std::string &name)
{
    this->shaders[name] = shader_create(("shaders/" + name + "_v.glsl").c_str(), ("shaders/" + name + "_f.glsl").c_str());
}


void RenderInfo::clear_shaders()
{
    for (auto &[key, value] : this->shaders)
        glDeleteShader(value);
}

