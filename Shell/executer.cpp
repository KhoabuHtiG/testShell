#include <functional>
#include <unordered_map>
#include "executer.h"

static std::unordered_map<std::string, std::function<void()>> cmds_ = {
    {"time", commands::printTime},
    {"exit", commands::exitShell},
    {"cmds", commands::cmds},
    {"cd..", commands::previousDirectory},
};

void executeCommand(const std::string& command) {
    if (cmds_.find(command) != cmds_.end()) {
        cmds_[command]();
    } else if (command == "dir" || command == "ls") {
        commands::getItemsInDirectory(std::filesystem::current_path().string());
    } else if (command.rfind("cd ", 0) == 0) {
        std::string path = command.substr(3);
        commands::nextDirectory(path);
    } else if(command == "cls" || command == "clear") {
        commands::clearScreen();
    } else if (command.rfind("start ", 0) == 0) {
        std::string program = command.substr(6);
        if (program.empty()) {
            printMessage("No input detect");
            return;
        }

        commands::startProgram(program);
    } else if (command.rfind("color ", 0) == 0) {
        std::string input = command.substr(6);
        if (input.empty()) {
            printMessage("No input detect");
            return;
        }

        char colorOption = input[0];
        commands::changeTextColor(colorOption);
    } else if (command.rfind("rename ", 0) == 0) {
        std::string file = command.substr(7);
        commands::renameFile(file);
    } else if (command.rfind("del ", 0) == 0) {
        std::string file = command.substr(4);
        if (file.empty()) {
            printMessage("No input detect.");
            return;
        }

        commands::deleteFile(file);
    } else if (command.empty()) {
        return;
    } else handleCommandError(command);

    return;
}