#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <glm/glm.hpp>

namespace util
{
    std::string read_file(const char *path);

    glm::vec3 restrict_angle(glm::vec3 angle);
    glm::vec3 restrict_cam_angle(glm::vec3 angle);
}

#endif

