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

fs::path getFileFolder() {return fs::path("../file");}
std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&time));

    return std::string(buf);
}

static std::vector<std::string> cmds_history;
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

void printMessage(const std::string message) {std::cout << message << '\n'; return;}
void executeCommand(const std::string& command);
void handleCommandError(const std::string& command) {
    printMessage(command + " : The term is not recognized as the name of a cmdlet, function, script file, or operable program."); return;}