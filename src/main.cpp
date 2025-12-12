#include "main.hpp"

int main(int argc, char **argv)
{
    bool isDebugModeEnabled = false;

    if (argc > 1) {
        if (std::string(argv[1]) == "--debug") {
            isDebugModeEnabled = true;
            std::cout << getColorFromCode(Color::YELLOW) << "[ARYA] - Launched with debug mode enabled." << getColorFromCode(Color::RESET) << std::endl;
        }
    }

    Arya arya(isDebugModeEnabled);
    arya.runArya();
    
    return 0;
};
