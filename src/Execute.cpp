#include "../include/Core.hpp"
#include "../include/Commands.hpp"

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