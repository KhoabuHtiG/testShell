#include <functional>
#include <unordered_map>
#include <filesystem>
#include "../include/executer.hpp"

namespace cmds_type {
    static std::unordered_map<std::string, std::function<void()>> reg_cmds = {
        {"time", commandType::regCommand::printTime},
        {"exit", commandType::regCommand::exitShell},
        {"help", commandType::regCommand::help},
        {"cls", commandType::regCommand::clearScreen},
        {"clear", commandType::regCommand::clearScreen},
        {"ver", commandType::regCommand::version},
        {"cmds", commandType::regCommand::cmds},
        {"cd..", commandType::regCommand::previousDirectory},
        {"create", commandType::regCommand::createFile},
        {"history", commandType::regCommand::showHistory},
        {"phistory", commandType::regCommand::readCmdHistory},
        {"whoami", commandType::regCommand::whoami},
        {"uptime", commandType::regCommand::upTime},
    };
    static std::unordered_map<std::string, std::function<void(std::string)>> arg_cmds {
        {"dir", [](const std::string& args) {
            commandType::argCommand::getItemsInDirectory(args.empty() ? fs::current_path().string() : args);
        }},
        {"ls", [](const std::string& args) {
            commandType::argCommand::getItemsInDirectory(args.empty() ? fs::current_path().string() : args);
        }},
        {"cd", [](const std::string& args) {
            commandType::argCommand::nextDirectory(args);
        }},
        {"start", [](const std::string& args) {
            if (args.empty()) { printMessage("No input detect"); return; }
            commandType::argCommand::startProgram(args);
        }},
        {"color", [](const std::string& args) {
            if (args.empty()) { printMessage("No input detect"); return; }
            commandType::argCommand::changeTextColor(args[0]);
        }},
        {"rename", [](const std::string& args) {
            commandType::argCommand::renameFile(args);
        }},
        {"del", [](const std::string& args) {
            if (args.empty()) { printMessage("No input detect"); return; }
            commandType::argCommand::deleteFile(args);
        }},
        {"echo", [](const std::string& args) {
            if (args.empty()) { printMessage("No input detect"); return; }
            commandType::argCommand::echo(args);
        }},
        {"tree", [](const std::string& args) {
            if (args.empty()){
                commandType::argCommand::tree(fs::current_path());
            } else {
                commandType::argCommand::tree(args);
            }
        }},
    };
}

void executeCommand(const std::string& input) {
    if (input.empty()) return;

    std::string cmd, args;
    size_t spacePos = input.find(' ');
    if (spacePos != std::string::npos) {
        cmd = input.substr(0, spacePos);
        args = input.substr(spacePos + 1);
    } else cmd = input;

    if (cmds_type::reg_cmds.find(cmd) != cmds_type::reg_cmds.end()) {
        cmds_type::reg_cmds.find(cmd)->second();
        addToHistory(input);
        return;
    }

    if (cmds_type::arg_cmds.find(cmd) != cmds_type::arg_cmds.end()) {
        cmds_type::arg_cmds.find(cmd)->second(args);
        addToHistory(input);
        return;
    }

    handleCommandError(cmd);
}