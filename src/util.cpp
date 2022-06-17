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

