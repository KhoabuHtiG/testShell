#pragma once
#include "Core.hpp"
#include "Commands/FileManagement.hpp"
#include "Commands/Navigation.hpp"
#include "Commands/System.hpp"
#include "cmds_doc.hpp"

namespace cmds_type {
    static std::unordered_map<std::string, std::function<void()>> reg_cmds = {
        {"time", commandType::systemCommand::printTime},
        {"exit", commandType::systemCommand::exitShell},
        {"clear", commandType::systemCommand::clearScreen},
        {"cls", commandType::systemCommand::clearScreen},
        {"cmds", commandType::systemCommand::listCmds},
        {"cd..", commandType::navigationCommand::returnToParentDirectory},
        {"his", commandType::systemCommand::readSessionHistoryCmds},
        {"phis", commandType::systemCommand::readPastHistoryCmds},
        {"whoami", commandType::systemCommand::whoami},
        {"uptime", commandType::systemCommand::uptime},
        {"path", commandType::navigationCommand::showCurrentPath},
    };
    static std::unordered_map<std::string, std::function<void(std::string)>> arg_cmds {
        {"ls", [](const std::string& args) {
            commandType::navigationCommand::listItemsInDirectory(args.empty() ? fs::current_path().string() : args);
        }},
        {"cd", [](const std::string& args) {
            printMessage("cd: Invalid format.");
            commandType::navigationCommand::accessDirectory(args);
        }},
        {"exec", [](const std::string& args) {
            if (args.empty()) { printMessage("exec: Invalid format."); return; }
            commandType::navigationCommand::executeProgram(args);
        }},
        {"color", [](const std::string& args) {
            if (args.empty()) { printMessage("color: Invalid format."); return; }
            commandType::systemCommand::changeTextColor(args[0]);
        }},
        {"rn", [](const std::string& args) {
            if (args.empty()) {printMessage("rn: Invalid format."); return;}

            size_t slashPos = args.find('/');
            if (slashPos == std::string::npos) {
                printMessage("rn: Invalid format.");
                return;
            }

            std::string fileName = args.substr(0, slashPos);
            std::string newName = args.substr(slashPos + 1);

            commandType::fileManagementCommand::renameFile(fileName, newName);
        }},
        {"del", [](const std::string& args) {
            if (args.empty()) {printMessage("del: Invalid format."); return; }
            commandType::fileManagementCommand::deleteFile(args);
        }},
        {"tree", [](const std::string& args) {
            if (args.empty()){
                commandType::navigationCommand::tree(fs::current_path());
            } else {
                commandType::navigationCommand::tree(args);
            }
        }},
        {"touch", [](const std::string& args) {
            if (args.empty()) {printMessage("touch: Invalid format."); return;};
            commandType::fileManagementCommand::makeFile(args);
        }},
        {"mkdir", [](const std::string& args) {
            if (args.empty()) {printMessage("mkdir: Invalid format."); return;};

            size_t slashPos = args.find('/');
            if (slashPos == std::string::npos) {
                commandType::fileManagementCommand::makeDirectory(args, "");
                return;
            }

            std::string mainDirec = args.substr(0, slashPos);
            std::string subDirec = args.substr(slashPos + 1);

            commandType::fileManagementCommand::makeDirectory(mainDirec, subDirec);
        }},
        {"type", [](const std::string& args) {
            if (args.empty()) {printMessage("type: Invalid format."); return;}

            size_t slashPos = args.find('/');
            if (slashPos == std::string::npos) {
                printMessage(args);
                return;
            }

            std::string fileName = args.substr(0, slashPos);
            std::string text = args.substr(slashPos + 1);

            commandType::fileManagementCommand::typeLineInFile(text, fileName);
        }},
        {"rd", [](const std::string& args) {
            if (args.empty()) {printMessage("rd: Invalid format."); return;}

            commandType::fileManagementCommand::readFileContent(args);
        }},
        {"help", [](const std::string& args) {
            if (args.empty()) {printMessage("help: Invalid format."); return;}

            auto it = cmds_doc.find(args);
            if (it != cmds_doc.end()) {
                printMessage(it->second);
            } else {
                printMessage("No manual entry for: " + args);
            }
        }},
        {"mv", [](const std::string& args) {
            if (args.empty()) {printMessage("mv: Invalid format."); return;}

            size_t slashPos = args.find('/');
            if (slashPos == std::string::npos) {
                printMessage("Invalid format.");
                return;
            }

            std::string fileName = args.substr(0, slashPos);
            std::string destination = args.substr(slashPos + 1);

            commandType::fileManagementCommand::moveFile(fileName, destination);
        }},
        {"dup", [](const std::string& args) {
            if (args.empty()) {printMessage("dup: Invalid format."); return;}

            size_t slashPos = args.find('/');
            if (slashPos == std::string::npos) {
                commandType::fileManagementCommand::duplicateFile(args, "");
                return;
            }

            std::string fileName = args.substr(0, slashPos);
            std::string destination = args.substr(slashPos + 1);

            commandType::fileManagementCommand::duplicateFile(fileName, destination);
        }}
    };
}