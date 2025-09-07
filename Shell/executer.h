#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <vector>

std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&time));

    return std::string(buf);
}

std::vector<std::string> commandList = {
    "time",
    "exit",
    "help",
    "cls/clear",
    "dir"
};

void printMessage(std::string message) {std::cout << message << '\n'; return;}
void executeCommand(const std::string& command);
void commandError(std::string command) {printMessage(command + " : The term '" +command+"' is not recognized as the name of a cmdlet, function, script file, or operable program."); return;}

class commands {
public:
    static void printTime() {printMessage(getTimestamp()); return;};
    static void exitShell() {printMessage("Exiting shell..."); exit(0);};

    static void help() {
        for (int i = 0; i < commandList.size(); i++) {
            printMessage(commandList[i]);
        }
    };

    static void getItemsInDirectory(std::string path) {
        try {
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                std::cout << entry.path() << std::endl;
            }
        } catch (const std::filesystem::filesystem_error& e) {
            printMessage("Error accessing directory: " + std::string(e.what()));
        }
    }

    static void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    };

};