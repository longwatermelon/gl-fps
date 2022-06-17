#include "shader.h"
#include "util.h"
#include <string>
#include <glad/glad.h>


unsigned int shader_create(const char *vert, const char *frag)
{
    std::string vertsrc = util::read_file(vert);
    std::string fragsrc = util::read_file(frag);

    unsigned int prog = glCreateProgram();
    unsigned int vs = shader_compile(GL_VERTEX_SHADER, vertsrc.c_str());
    unsigned int fs = shader_compile(GL_FRAGMENT_SHADER, fragsrc.c_str());

    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    glValidateProgram(prog);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return prog;
}


unsigned int shader_compile(unsigned int type, const char *src)
{
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, 0);
    glCompileShader(id);

    int res;
    glGetShaderiv(id, GL_COMPILE_STATUS, &res);

    if (res == GL_FALSE)
    {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char *err = (char*)alloca(sizeof(char) * len);
        glGetShaderInfoLog(id, len, &len, err);

        printf("Failed to compile %s shader:\n%s\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment", err);

        glDeleteShader(id);
        return 0;
    }

    return id;
}

