#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

namespace FS {
namespace Utils {

std::vector<std::string> split(std::string str)
{
    std::stringstream ss(str);
    std::vector<std::string> answer;
    std::string token;
    while (ss >> token)
    {
        answer.push_back(token);
    }
    return answer;
}

void lengthen(std::string& file_name)
{
    while (file_name.size() < 4)
    {
        file_name += ' ';
    }
}
}
} // namespace FS