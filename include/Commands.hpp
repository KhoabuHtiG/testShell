#pragma once
#include "Core.hpp"
#include "Commands/FileManagement.hpp"
#include "Commands/Navigation.hpp"
#include "Commands/System.hpp"

namespace cmds_type {
    static std::unordered_map<std::string, std::function<void()>> reg_cmds = {
        {"time", commandType::systemCommand::printTime},
        {"exit", commandType::systemCommand::exitShell},
        {"help", commandType::systemCommand::help},
        {"cls", commandType::systemCommand::clearScreen},
        {"cmds", commandType::systemCommand::listCmds},
        {"cp..", commandType::navigationCommand::returnToParentDirectory},
        {"his", commandType::systemCommand::readSessionHistoryCmds},
        {"phis", commandType::systemCommand::readPastHistoryCmds},
        {"whoami", commandType::systemCommand::whoami},
        {"uptime", commandType::systemCommand::uptime},
    };
    static std::unordered_map<std::string, std::function<void(std::string)>> arg_cmds {
        {"list", [](const std::string& args) {
            commandType::navigationCommand::listItemsInDirectory(args.empty() ? fs::current_path().string() : args);
        }},
        {"cp", [](const std::string& args) {
            commandType::navigationCommand::accessDirectory(args);
        }},
        {"exec", [](const std::string& args) {
            if (args.empty()) { printMessage("No input detect"); return; }
            commandType::navigationCommand::executeProgram(args);
        }},
        {"color", [](const std::string& args) {
            if (args.empty()) { printMessage("No input detect"); return; }
            commandType::systemCommand::changeTextColor(args[0]);
        }},
        {"rename", [](const std::string& args) {
            commandType::fileManamentCommand::renameFile(args);
        }},
        {"del", [](const std::string& args) {
            if (args.empty()) { printMessage("No input detect"); return; }
            commandType::fileManamentCommand::deleteFile(args);
        }},
        {"tree", [](const std::string& args) {
            if (args.empty()){
                commandType::navigationCommand::tree(fs::current_path());
            } else {
                commandType::navigationCommand::tree(args);
            }
        }},
        {"makef", [](const std::string& args) {
            if (args.empty()) {printMessage("No input found"); return;};
            commandType::fileManamentCommand::makeFile(args);
        }},
        {"makedirec", [](const std::string& args) {
            if (args.empty()) {printMessage("No input found"); return;};
            commandType::fileManamentCommand::makeDirectory(args);
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

            commandType::fileManamentCommand::typeLineInFile(text, fileName);
        }}
    };
}