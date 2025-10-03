#pragma once
#include "../Core.hpp"

std::vector<std::string> commandList = {
    "|| help", "|| time", "|| exit", "|| cmds", "|| clear/cls",
    "|| ls", "|| cd", "|| cd..", "|| exec", "|| color", "|| rn",
    "|| del", "|| his", "|| phis", "|| whoami", "|| uptime",
    "|| tree", "|| touch", "|| mkdir", "|| type", "|| path",
    "|| rd", "|| mv", "|| dup", "prop",
};
static std::vector<std::string> colors {
    "   1 = Red             9 = Bold",
    "   2 = Yellow          A = Bright Red",
    "   3 = Blue            B = Bright Yellow",
    "   4 = Green           C = Light Aqua",
    "   5 = White           D = Light Red",
    "   6 = Cyan            E = Light Purple",
    "   7 = Grey            F = Light Yellow",
    "   8 = Magenta         G = Bright White",
};
static std::unordered_map<char, std::ostream& (*)(std::ostream&)> color_ = {
    {'1', termcolor::red},
    {'2', termcolor::yellow},
    {'3', termcolor::blue},
    {'4', termcolor::green},
    {'5', termcolor::white},
    {'6', termcolor::cyan},
    {'7', termcolor::grey},
    {'8', termcolor::magenta},
    {'9', termcolor::bold},
    {'a', termcolor::bright_red},
    {'A', termcolor::bright_red},
    {'b', termcolor::bright_yellow},
    {'B', termcolor::bright_yellow},
};

namespace commandType {
    class systemCommand {
    public:
        static void printTime() {printMessage("Current time: " + getTimestamp());};
        static void exitShell() {printMessage("Exiting shell..."); exit(0);};
        static void listCmds() {
            for (int i = 0; i < commandList.size(); i++) printMessage(commandList[i]);
        };
        static void clearScreen() {
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
        };
        static void whoami() {
            #ifdef _WIN32
                char username[UNLEN + 1];
                DWORD username_len = UNLEN + 1;
                if (GetUserNameA(username, &username_len)) {
                    std::cout << username << "\n";
                } else std::cerr << "Error getting username.\n";
            #elif __linux__
                const char* username = getenv("USER"); // env var
                if (username) {
                    std::cout << username << "\n";
                } else {
                    struct passwd* pw = getpwuid(getuid());
                    if (pw) {
                        std::cout << pw->pw_name << "\n";
                    } else {
                        std::cerr << "Error getting username.\n";
                    }
                }
            #endif
        }
        static void uptime() {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - shellStartTime).count();

            int hours = elapsed / 3600;
            int minutes = (elapsed % 3600) / 60;
            int seconds = elapsed % 60;

            printMessage("Uptime: " + std::to_string(hours) + "h " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s ");
        }
        static void changeTextColor(char option) {
            auto it = color_.find(option);
            if (it != color_.end()) {
                std::cout << it->second;
            } else for (int i = 0; i < colors.size(); ++i) printMessage(colors[i]);
    
            return;
        }
        static void readSessionHistoryCmds() {
            for (int i = 0; i < cmds_history.size(); ++i) printMessage(cmds_history[i]);
        }
        static void readPastHistoryCmds() {
            std::ifstream file(getFileFolder() / "Cmd_History_Log.txt");
            if (!file.is_open()) printMessage("phis: Failed to open log file.");

            std::string line;
            while (std::getline(file, line)) printMessage(line);
        }
    };
}