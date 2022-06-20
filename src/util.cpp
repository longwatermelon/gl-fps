#include "util.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <random>


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


glm::vec3 util::barycentric_coeffs(std::array<glm::vec3, 3> points, glm::vec3 p)
{
    glm::vec3 ba = points[1] - points[0];
    glm::vec3 ca = points[2] - points[0];

    glm::vec3 pa = p - points[0];
    glm::vec3 pb = p - points[1];
    glm::vec3 pc = p - points[2];

    glm::vec3 n = glm::cross(ba, ca);
    float s_abc = glm::length(n);
    n = glm::normalize(n);

    glm::vec3 pbc = glm::cross(pb, pc);
    float s_pbc = glm::dot(n, pbc);

    glm::vec3 apc = glm::cross(pa, ca);
    float s_apc = glm::dot(n, apc);

    glm::vec3 abp = glm::cross(ba, pa);
    float s_abp = glm::dot(n, abp);

    return glm::vec3(
        s_pbc / s_abc,
        s_apc / s_abc,
        s_abp / s_abc
    );
}


float util::point_line_shortest_dist(glm::vec3 p, glm::vec3 p0, glm::vec3 p1)
{
    float len = glm::length(p1 - p0);
    float t = glm::dot(p1 - p0, p - p0) / len;

    if (t < 0.f) return glm::length(p - p0);
    if (t > len) return glm::length(p - p1);

    float h = glm::length(p - p0);
    return sqrtf(h * h - t * t);
}


int util::randint(int min, int max)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(min, max);

    return uni(rng);
}


glm::vec3 util::randvec(int min, int max)
{
    return glm::vec3(
        util::randint(min, max),
        util::randint(min, max),
        util::randint(min, max)
    );
}

