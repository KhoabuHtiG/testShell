#include "../include/Core.hpp"
#include "Execute.cpp"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    clearScreen();
    std::cout << "Use 'cmds' to get the list of available commands.\n";

    while (true) {
        std::string input;
        std::cout << fs::current_path() << ":> ";

        if (std::getline(std::cin, input)) {
            executeCommand(input);
            continue;
        }
    }
}