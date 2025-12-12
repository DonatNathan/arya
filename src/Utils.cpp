#include "Utils.hpp"

std::string getColorFromCode(Color colorCode)
{
    int code = static_cast<int>(colorCode);
    return "\033[1;" + std::to_string(code) + "m";
};
