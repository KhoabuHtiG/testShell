#include <functional>
#include <unordered_map>
#include "executer.h"

//Run command function
void executeCommand(const std::string& command) {
    //Commands
    std::unordered_map<std::string, std::function<void()>> cmds_ = {
        {"time", commands::printTime},
        {"exit", commands::exitShell},
        {"cmds", commands::cmds},
        {"cd..", commands::previousDirectory},
    };

    //Run commands
    if (cmds_.find(command) != cmds_.end()) {
        cmds_[command]();
    } else if (command == "dir") {
        commands::getItemsInDirectory(std::filesystem::current_path().string());
    } else if (command.rfind("cd ", 0) == 0) {
        std::string path = command.substr(3);
        if (!commands::nextDirectory(path)) {
            printMessage("The " + path + " is not recognized as a path, a file or a directory.");
        } 
    } else if(command == "cls" || command == "clear") {
        commands::clearScreen();
    } else if (command.rfind("start ", 0) == 0) {
        std::string program = command.substr(6);
        if (!commands::startProgram(program)) {
            printMessage("The " + program + " is not recognized as a possible executed file.");
        }
    } else if (command.empty()) {
        return;
    } else {
        handleCommandError(command);
    }
    return;
}