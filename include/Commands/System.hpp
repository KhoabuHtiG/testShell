#pragma once
#include "../Core.hpp"

std::vector<std::string> commandList = {
    "|| help", "|| time", "|| exit", "|| cmds", "|| clear/cls",
    "|| ls", "|| cd", "|| cd..", "|| exec", "|| color", "|| rn",
    "|| del", "|| his", "|| phis", "|| whoami", "|| uptime",
    "|| tree", "|| touch", "|| mkdir", "|| type", "|| path",
    "|| rd", "|| mv",
};
static std::vector<std::string> colors {
    "   0 = Black         8 = Gray",
    "   1 = Blue          9 = Light Blue",
    "   2 = Green         A = Light Green",
    "   3 = Aqua          B = Light Aqua",
    "   4 = Red           C = Light Red",
    "   5 = Purple        D = Light Purple",
    "   6 = Yellow        E = Light Yellow",
    "   7 = White         F = Bright White",
};
static std::unordered_map<char, std::function<int()>> color_ = {
    {'0', []() {return system("Color 00");}},
    {'1', []() {return system("Color 01");}},
    {'2', []() {return system("Color 02");}},
    {'3', []() {return system("Color 03");}},
    {'4', []() {return system("Color 04");}},
    {'5', []() {return system("Color 05");}},
    {'6', []() {return system("Color 06");}},
    {'7', []() {return system("Color 07");}},
    {'8', []() {return system("Color 08");}},
    {'9', []() {return system("Color 09");}},
    {'A', []() {return system("Color 0A");}},
    {'B', []() {return system("Color 0B");}},
    {'C', []() {return system("Color 0C");}},
    {'D', []() {return system("Color 0D");}},
    {'E', []() {return system("Color 0E");}},
    {'F', []() {return system("Color 0F");}},
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
                printMessage("Use 'cmds' to get list of commands.");
            #else
                system("clear");
                printMessage("Use 'cmds' to get list of commands.");
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
            if (color_.find(option) != color_.end()) {
                color_[option]();
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
