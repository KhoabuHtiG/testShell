#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <filesystem>
#include <functional>
#include <vector>
#include <stdlib.h>
#include <unordered_map>
namespace fs = std::filesystem;

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
static std::vector<std::string> commandList = {
    "|| time: Show current date and time     ||",
    "|| exit: Exit the shell                 ||",
    "|| cmds: Show list of commands          ||",
    "|| cls/clear: Clear the screen          ||",
    "|| dir: List items in current directory ||",
    "|| cd: Access to the exist directory    ||",
    "|| cd..: Go to previous directory       ||",
    "|| start: Start a exe program           ||",
    "|| color: Change the color of the text  ||",
    "|| rename: Change the name of a file    ||",
    "|| del: Delete file or directory        ||",
};

class commands {
public:
    static inline std::unordered_map<char, std::function<int()>> color_ = {
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

    static void printTime() {printMessage(getTimestamp()); return;};
    static void exitShell() {printMessage("Exiting shell..."); exit(0);};
    static void cmds() {
        for (int i = 0; i < commandList.size(); i++) {
            printMessage(commandList[i]);
        }
    };
    static void getItemsInDirectory(const std::string path) {
        try {
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_directory()) {
                    printMessage("|| <DIR>   " + entry.path().filename().string());
                } else if (entry.is_regular_file()) {
                    printMessage("||         " + entry.path().filename().string());
                } else {
                    printMessage("|| <OTHER> " + entry.path().filename().string());
                }
            }
        } catch (const fs::filesystem_error& e) {
            printMessage("Error accessing directory: " + std::string(e.what()));
        }
    }
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
            std::string error = e.what();
            printMessage("Error while remaming file: " + error);
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
};