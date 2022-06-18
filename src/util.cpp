#include "util.h"
#include <sstream>
#include <fstream>
#include <iostream>


std::string util::read_file(const char *path)
{
    std::ifstream ifs(path);
    std::stringstream ss;
    std::string line;

    while (std::getline(ifs, line))
        ss << line << "\n";

    ifs.close();

    return ss.str();
}


glm::vec3 util::restrict_angle(glm::vec3 angle)
{
    glm::vec3 deg = glm::degrees(angle);

    if (deg.x > 360.f) deg.x -= 360.f;
    if (deg.x < 0.f) deg.x += 360.f;

    if (deg.y > 360.f) deg.y -= 360.f;
    if (deg.y < 0.f) deg.y += 360.f;

    if (deg.z > 360.f) deg.z -= 360.f;
    if (deg.z < 0.f) deg.z += 360.f;

    return glm::radians(deg);
}


glm::vec3 util::restrict_cam_angle(glm::vec3 angle)
{
    glm::vec3 deg = glm::degrees(angle);

    if (deg.z > 90.f) deg.z = 90.f;
    if (deg.z < -90.f) deg.z = -90.f;

    return glm::radians(deg);
}

