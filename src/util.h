#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <array>
#include <glm/glm.hpp>

namespace util
{
    std::string read_file(const char *path);

    glm::vec3 restrict_angle(glm::vec3 angle);
    glm::vec3 restrict_cam_angle(glm::vec3 angle);

    glm::vec3 barycentric_coeffs(std::array<glm::vec3, 3> points, glm::vec3 p);
    float point_line_shortest_dist(glm::vec3 p, glm::vec3 p0, glm::vec3 p1);

    int randint(int min, int max);
}

#endif

