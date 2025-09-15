#include <iostream>
#include <string>
#include "executer.cpp"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    clearScreen();
    std::cout << "Type 'cmds' to get list of commands.\n";

    while (true) {
        std::string input;
        std::filesystem::path currentPath = std::filesystem::current_path();
        std::cout << currentPath << ":> ";

        if (std::getline(std::cin, input)) {
            executeCommand(input);
            continue;
        }
    }
}