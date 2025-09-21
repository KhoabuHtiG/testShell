#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <functional>
#include <vector>
#include <unordered_map>
#ifdef _WIN32
    #include <windows.h>
    #include <Lmcons.h>
#elif __linux__
    #include <unistd.h>
    #include <pwd.h>
#endif

namespace fs = std::filesystem;
static auto shellStartTime = std::chrono::steady_clock::now();

std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&time));

    return std::string(buf);
}

void printMessage(const std::string message) {std::cout << message << '\n'; return;}
void executeCommand(const std::string& command);
void handleCommandError(const std::string& command) {
    printMessage(command + " : The term '" +command+"' is not recognized as the name of a cmdlet, function, script file, or operable program."); return;}

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
std::vector<std::string> commandList = {
    "|| help", "|| time", "|| exit", "|| cmds", "|| ver", "|| cls / clear",
    "|| dir / ls", "|| cd", "|| cd..", "|| start", "|| color", "|| rename",
    "|| create", "|| del", "|| history", "|| phistory", "|| whoami",
    "|| uptime", "|| echo", "|| tree",
};

uintmax_t getDirectorySize(const fs::path& directoryPath) {
    uintmax_t totalSize = 0;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
            if (fs::is_regular_file(entry.path())) {
                totalSize += fs::file_size(entry.path());
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return totalSize;
}
static std::vector<std::string> cmds_history;
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

fs::path getFileFolder() {return fs::path("../files");}

static void addToHistory(const std::string cmd) {
    cmds_history.emplace_back(cmd);

    fs::path fileFolder = getFileFolder();
    fs::path fileName = fileFolder / "Cmd_History_Log.txt";
    if (!fs::exists(fs::path(fileName))) {
        std::ofstream historyFile(fileName);
        historyFile.close();
    }

    std::ofstream historyLogFile(fileName, std::ios::app);
    if (historyLogFile.is_open()) {
        historyLogFile << cmd << '\n';
        historyLogFile.close();
    }
}
static void printTree(const fs::path& path, int depth = 0) {
    for (int i = 0; i < depth; i++) {
        std::cout << "   ";
    }

    std::cout << "|-- " << path.filename().string() << "\n";

    if (fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            printTree(entry.path(), depth + 1);
        }
    }
}

namespace commandType {
    class regCommand {
    public:
        static void help() {
            std::ifstream file(getFileFolder() / "HelpCommandList.txt");
            if (file.is_open()) {
                std::string line;

                while (std::getline(file, line)) {
                    printMessage(line);
                }
            }
        }
        static void printTime() {printMessage(getTimestamp());};
        static void exitShell() {printMessage("Exiting shell..."); exit(0);};
        static void version() {printMessage("TestShell v0.0.0 - Sep 7 2025");};
        static void cmds() {
            for (int i = 0; i < commandList.size(); i++) {
                printMessage(commandList[i]);
            }
        };
        static void clearScreen() {
            #ifdef _WIN32
                system("cls");
                printMessage("Type 'cmds' to get list of commands.");
            #else
                system("clear");
                printMessage("Type 'cmds' to get list of commands.");
            #endif
        };
        static void previousDirectory() {
            fs::current_path(fs::current_path().parent_path());
            return;
        };
        static void createFile() {
            try {
                std::string input;
                char option;

                printMessage("Would you like to create a file or a directory(f, d): ");
                std::getline(std::cin, input);
                if (input.empty()) {
                    printMessage("No input detect.");
                    return;
                }

                option = input[0];
                if (option == 'f' || option == 'F') {
                    std::string fileName;
                    printMessage("Please enter your file name: ");
                    std::getline(std::cin, fileName);
                    if (fileName.empty()) {
                        printMessage("No input detect.");
                        return;
                    }

                    if (fs::exists(fileName)) {
                        printMessage("The file is already existed");
                        return;
                    }

                    std::ofstream file(fileName);
                    printMessage("Created "+fileName);
                } else if (option == 'd' || option == 'D') {
                    std::string directoryName;
                    printMessage("Please enter your directory name: ");
                    std::getline(std::cin, directoryName);
                    if (directoryName.empty()) {
                        printMessage("No input detect.");
                        return;
                    }

                    fs::create_directory(directoryName);
                    printMessage("Created "+directoryName);
                } else {
                    printMessage("Invalid input.");
                    return;
                }
            } catch (fs::filesystem_error& e) {
                printMessage("Error creating a file or directory: " + std::string(e.what()));
            }
            return;
        }
        static void showHistory() {
            for (int i = 0; i < cmds_history.size(); ++i) {
                printMessage(cmds_history[i]);
            }
        }
        static void readCmdHistory() {
            std::vector<std::string> loadData;
            std::ifstream historyLogFile(getFileFolder() / "Cmd_History_Log.txt");

            if (historyLogFile.is_open()) {
                std::string line;
                while (std::getline(historyLogFile, line)) { 
                    loadData.push_back(line);
                }
                historyLogFile.close();
            }

            for (const auto& cmd : loadData) {
                printMessage(cmd);
            }
        }
        static void whoami() {
            #ifdef _WIN32
                char username[UNLEN + 1];
                DWORD username_len = UNLEN + 1;
                if (GetUserNameA(username, &username_len)) {
                    std::cout << username << "\n";
                } else {
                    std::cerr << "Error getting username.\n";
                }
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
        static void upTime() {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - shellStartTime).count();

            int hours = elapsed / 3600;
            int minutes = (elapsed % 3600) / 60;
            int seconds = elapsed % 60;

            printMessage("Uptime: " + std::to_string(hours) + "h " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s ");
        }
    };
    class argCommand {
    public:
        static void getItemsInDirectory(const std::string path) {
            try {
                printMessage("\n      Directory: " + path + '\n');
                for (const auto& entry : fs::directory_iterator(path)) {
                    if (entry.is_directory()) {
                        uintmax_t size = getDirectorySize(entry);
                        printMessage("|| <DIR>   " + entry.path().filename().string() + " | Length: " + std::to_string(size));
                    } else if (entry.is_regular_file()) {
                        printMessage("||         " + entry.path().filename().string() + " | Length: " + std::to_string(fs::file_size(entry.path())));
                    } else {
                        printMessage("|| <OTHER> " + entry.path().filename().string() + " | Length: " + std::to_string(fs::file_size(entry.path())));
                    }
                }
            } catch (const fs::filesystem_error& e) {
                printMessage("Error accessing directory: " + std::string(e.what()));
            }
        }
        static bool nextDirectory(const std::string path) {
            try {
                if (fs::exists(path) && fs::is_directory(path)) {
                    fs::current_path(path);
                    return true;
                } else {
                    printMessage("'"+path+"' is not recognized as a path, a file or a directory.");
                    return false;
                }
            } catch (const fs::filesystem_error& e) {
                printMessage("Error accessing directory: " + std::string(e.what()));
                return false;
            }
        }
        static void startProgram(const std::string program) {
            try {
                if (fs::exists(program) && fs::is_regular_file(program)) {
                    #ifdef _WIN32
                        std::string cmd = "start \"\" \"" + program + "\"";
                    #else
                        std::string cmd = "./" + program;
                    #endif
                    system(cmd.c_str());
                } else {
                    printMessage("The " + program + " is not recognized as a possible executed file.");
                }
            } catch (fs::filesystem_error &e) {
                printMessage("Error starting program: " + std::string(e.what()));
            }
            return;
        }
        static void changeTextColor(char option) {
            if (color_.find(option) != color_.end()) {
                color_[option]();
            } else {
                for (int i = 0; i < colors.size(); ++i) {
                    printMessage(colors[i]);
                }
            }
            return;
        }
        static void renameFile(const std::string path) {
            try {
                if (fs::exists(path)) {
                    std::string newFileName, oldFileName = path;
                    printMessage("Please enter your new name: ");
                    std::getline(std::cin, newFileName);

                    if (std::rename(oldFileName.c_str(), newFileName.c_str()) != 0) {
                        printMessage("Error while renaming name.");
                    } else {
                        printMessage("Rename file successfully. " + oldFileName + " to " + newFileName);
                    }
                } else {
                    printMessage("The "+path+" is not recognized as a path, a file or a directory.");
                }
            } catch (fs::filesystem_error& e) {
                printMessage("Error while remaming file: " + std::string(e.what()));
            }
            return;
        }
        static void deleteFile(const std::string path) {
            try {
                if (fs::exists(path)) {
                    if (fs::is_regular_file(path)) {
                        fs::remove(path);
                        printMessage("Removed "+path+" successfully");
                    } else if (fs::is_directory(path)) {
                        fs::remove_all(path);
                        printMessage("Removed "+path+" successfully");
                    }
                } else {
                    printMessage("The "+path+" is not recognized as a path, a file or a directory.");
                }
            } catch (fs::filesystem_error& e) {
                std::string error = e.what();
                printMessage("Error deleting file or directory: " + error);
            }
            return;
        }
        static void echo(const std::string input) {
            printMessage(input);
        }
        static void tree(const fs::path path) {
            fs::path root = path.empty() ? fs::current_path() : fs::path(path);

            if (!fs::exists(root)) {
                std::cout << "Path not found: " << root << "\n";
                return;
            }

            std::cout << root.string() << "\n";
            for (const auto& entry : fs::directory_iterator(root)) {
                printTree(entry.path(), 1);
            }
        }
    };
}