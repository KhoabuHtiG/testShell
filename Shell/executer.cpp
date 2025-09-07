#include <iostream>
#include "executer.h"

void executeCommand(const std::string& command) {
    if (command == "time") {
        commands::printTime();
    } else if (command == "exit" || command == "quit") {
        commands::exitShell();
    } else if (command == "cmds") {
        commands::cmds();
    } else if (command == "cls" || command == "clear") {
        commands::clearScreen();
        printMessage("Type 'cmds' to get list of commands.");
    } else if (command == "dir") {
        commands::getItemsInDirectory(std::filesystem::current_path().string());
    } else if (command == "cd..") {
        commands::previousDirectory();
    } else if (command.empty()) {
        return;
    } else {
        commandError(command);
    }
    return;
}
