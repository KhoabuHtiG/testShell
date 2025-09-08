#include <iostream>
#include <string>
#include <filesystem>
#include "executer.cpp"

//Clear screen function
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

//Main function
int main() {
    clearScreen();
    std::cout << "Welcome to a test shell!\n";
    std::cout << "Type 'cmds' to get list of commands.\n";

    //Shell looping
    while (true) {
        std::string input;
        //Get the current path of the shell
        std::filesystem::path currentPath = std::filesystem::current_path();
        std::cout << currentPath << " :> ";

        if (std::getline(std::cin, input)) {
            executeCommand(input);
            continue;
        }
    }
}