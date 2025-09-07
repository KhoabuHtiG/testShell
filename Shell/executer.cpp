#include <iostream>
#include "executer.h"

void executeCommand(const std::string& command) {
    if (command == "time") {
        commands::printTime();
    } else if (command == "exit") {
        commands::exitShell();
    } else if (command == "help") {
        commands::help();
    } else if (command == "cls" || command == "clear") {
        commands::clearScreen();
        printMessage("Type 'help' to get list of commands.\n");
    } else if (command == "dir") {
        commands::getItemsInDirectory(std::filesystem::current_path().string());
    } else if (command.empty()) {

    } else {
        commandError(command);
    }
    return;
}