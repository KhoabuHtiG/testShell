#include <functional>
#include <unordered_map>
#include <filesystem>
#include "../include/executer.h"

static std::unordered_map<std::string, std::function<void()>> reg_cmds = {
    {"time", commands::printTime},
    {"exit", commands::exitShell},
    {"help", commands::help},
    {"ver", commands::version},
    {"cmds", commands::cmds},
    {"cd..", commands::previousDirectory},
    {"create", commands::createFile},
    {"history", commands::showHistory},
    {"phistory", commands::readCmdHistory},
};

void executeCommand(const std::string& command) {
    if (reg_cmds.find(command) != reg_cmds.end()) {
        reg_cmds[command]();
        addToHistory(command);
    } else if (command == "dir" || command == "ls") {
        commands::getItemsInDirectory(fs::current_path().string());
        addToHistory(command);
    } else if (command.rfind("cd ", 0) == 0) {
        std::string path = command.substr(3);
        commands::nextDirectory(path);
        addToHistory(command + path);
    } else if(command == "cls" || command == "clear") {
        commands::clearScreen();
        addToHistory(command);
    } else if (command.rfind("start ", 0) == 0) {
        std::string program = command.substr(6);
        if (program.empty()) {
            printMessage("No input detect");
            return;
        }

        commands::startProgram(program);
        addToHistory(command + program);
    } else if (command.rfind("color ", 0) == 0) {
        std::string input = command.substr(6);
        if (input.empty()) {
            printMessage("No input detect");
            return;
        }

        char colorOption = input[0];
        commands::changeTextColor(colorOption);
        addToHistory(command + colorOption);
    } else if (command.rfind("rename ", 0) == 0) {
        std::string file = command.substr(7);
        commands::renameFile(file);
        addToHistory(command + file);
    } else if (command.rfind("del ", 0) == 0) {
        std::string file = command.substr(4);
        if (file.empty()) {
            printMessage("No input detect.");
            return;
        }

        commands::deleteFile(file);
        addToHistory(command + file);
    } else if (command.empty()) {
        return;
    } else handleCommandError(command);

    return;
}