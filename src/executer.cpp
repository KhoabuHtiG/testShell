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
        {"cmds", commandType::regCommand::listCmds},
        {"cp..", commandType::regCommand::returnToParentDirectory},
        {"his", commandType::regCommand::showHistory},
        {"phis", commandType::regCommand::readCmdHistory},
        {"whoami", commandType::regCommand::whoami},
        {"uptime", commandType::regCommand::uptime},
    };
    static std::unordered_map<std::string, std::function<void(std::string)>> arg_cmds {
        {"list", [](const std::string& args) {
            commandType::argCommand::listItemsInDirectory(args.empty() ? fs::current_path().string() : args);
        }},
        {"cp", [](const std::string& args) {
            commandType::argCommand::accessDirectory(args);
        }},
        {"exec", [](const std::string& args) {
            if (args.empty()) { printMessage("No input detect"); return; }
            commandType::argCommand::executeProgram(args);
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
        {"tree", [](const std::string& args) {
            if (args.empty()){
                commandType::argCommand::tree(fs::current_path());
            } else {
                commandType::argCommand::tree(args);
            }
        }},
        {"makef", [](const std::string& args) {
            if (args.empty()) {printMessage("No input found"); return;};
            commandType::argCommand::makeFile(args);
        }},
        {"makedirec", [](const std::string& args) {
            if (args.empty()) {printMessage("No input found"); return;};
            commandType::argCommand::makeDirectory(args);
        }},
        {"type", [](const std::string& args) {
            if (args.empty()) {printMessage("Invalid format. Use: type <file>/<text>"); return;}

            size_t slashPos = args.find('/');
            if (slashPos == std::string::npos) {
                printMessage("Invalid format. Use: type <file>/<text>");
                return;
            }

            std::string fileName = args.substr(0, slashPos);
            std::string text = args.substr(slashPos + 1);

            commandType::argCommand::typeLineInFile(text, fileName);
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