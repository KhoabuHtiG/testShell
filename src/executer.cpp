#include <functional>
#include <unordered_map>
#include <filesystem>
#include "../include/executer.hpp"

namespace cmds_type {
    static std::unordered_map<std::string, std::function<void()>> reg_cmds = {
        {"time", regCommand::printTime},
        {"exit", regCommand::exitShell},
        {"help", regCommand::help},
        {"cls", regCommand::clearScreen},
        {"clear", regCommand::clearScreen},
        {"ver", regCommand::version},
        {"cmds", regCommand::cmds},
        {"cd..", regCommand::previousDirectory},
        {"create", regCommand::createFile},
        {"history", regCommand::showHistory},
        {"phistory", regCommand::readCmdHistory},
        {"whoami", regCommand::whoami},
        {"uptime", regCommand::upTime},
    };

    static std::unordered_map<std::string, std::function<void(std::string)>> arg_cmds {
        {"dir", [](const std::string& args) {
            argCommand::getItemsInDirectory(args.empty() ? fs::current_path().string() : args);
        }},
        {"ls", [](const std::string& args) {
            argCommand::getItemsInDirectory(args.empty() ? fs::current_path().string() : args);
        }},
        {"cd", [](const std::string& args) {
            argCommand::nextDirectory(args);
        }},
        {"start", [](const std::string& args) {
            if (args.empty()) { printMessage("No input detect"); return; }
            argCommand::startProgram(args);
        }},
        {"color", [](const std::string& args) {
            if (args.empty()) { printMessage("No input detect"); return; }
            argCommand::changeTextColor(args[0]);
        }},
        {"rename", [](const std::string& args) {
            argCommand::renameFile(args);
        }},
        {"del", [](const std::string& args) {
            if (args.empty()) { printMessage("No input detect"); return; }
            argCommand::deleteFile(args);
        }},
        {"echo", [](const std::string& args) {
            if (args.empty()) { printMessage("No input detect"); return; }
            argCommand::echo(args);
        }}
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

    auto regCmd = cmds_type::reg_cmds.find(cmd);
    if (regCmd != cmds_type::reg_cmds.end()) {
        regCmd->second();
        addToHistory(input);
        return;
    }

    auto argCmd = cmds_type::arg_cmds.find(cmd);
    if (cmds_type::arg_cmds.find(cmd) != cmds_type::arg_cmds.end()) {
        argCmd->second(args);
        addToHistory(input);
        return;
    }

    handleCommandError(cmd);
}
