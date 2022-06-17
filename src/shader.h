#pragma once

#define shader_vec3(shader, name, value) glUniform3f(glGetUniformLocation(shader, name.c_str()), value.x, value.y, value.z)
#define shader_mat4(shader, name, value) glUniformMatrix4fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, value)
#define shader_float(shader, name, value) glUniform1f(glGetUniformLocation(shader, name.c_str()), value)
#define shader_int(shader, name, value) glUniform1i(glGetUniformLocation(shader, name.c_str()), value)

unsigned int shader_create(const char *vert, const char *frag);
unsigned int shader_compile(unsigned int type, const char *src);

