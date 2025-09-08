#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;

//Get the current time in real life 
std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&time));

    return std::string(buf);
}

//Commands menu
std::vector<std::string> commandList = {
    "|| time: Show current date and time     ||",
    "|| exit: Exit the shell                 ||",
    "|| cmds: Show list of commands          ||",
    "|| cls/clear: Clear the screen          ||",
    "|| dir: List items in current directory ||",
    "|| cd: Access to the exist directory    ||",
    "|| cd..: Go to previous directory       ||",
};

//Print message function
void printMessage(std::string message) {std::cout << message << '\n'; return;}
//Run command function
void executeCommand(const std::string& command);
//Error handler
void handleCommandError(std::string command) {printMessage(command + " : The term '" +command+"' is not recognized as the name of a cmdlet, function, script file, or operable program."); return;}

//Commands
class commands {
public:
    //Print the time function
    static void printTime() {printMessage(getTimestamp()); return;};
    //Quit shell function
    static void exitShell() {printMessage("Exiting shell..."); exit(0);};
    //Show the list of available commands function
    static void cmds() {
        //Print the commands list
        for (int i = 0; i < commandList.size(); i++) {
            printMessage(commandList[i]);
        }
    };
    //Get files or directories that included in the path function
    static void getItemsInDirectory(std::string path) {
        try {
            for (const auto& entry : fs::directory_iterator(path)) {
                //Check what sort of the file is
                if (entry.is_directory()) { //Directory
                    printMessage("|| <DIR>   " + entry.path().filename().string());
                } else if (entry.is_regular_file()) { //Regular file
                    printMessage("||         " + entry.path().filename().string());
                } else { //Unknown file 
                    printMessage("|| <OTHER> " + entry.path().filename().string());
                }
            }
        } catch (const fs::filesystem_error& e) { //Handle error
            printMessage("Error accessing directory: " + std::string(e.what()));
        }
    }
    //Clear screen function
    static void clearScreen() {
        //Case if the user is using Windows
        #ifdef _WIN32
            system("cls");
            printMessage("Type 'cmds' to get list of commands.");
        #else //Case if the user is using other (Linux, macOS, etc...)
            system("clear");
            printMessage("Type 'cmds' to get list of commands.");
        #endif
    };
    //Return to the parent of the present path function
    static void previousDirectory() {
        fs::current_path(fs::current_path().parent_path());
        return;
    };
    //Access to the available path function
    static bool nextDirectory(std::string path) {
        try {
            //Check if the path is a directory or not
            if (fs::exists(path) && fs::is_directory(path)) {
                fs::current_path(path);
                return true;
            } else {
                return false;
            }
        } catch (const fs::filesystem_error& e) { //Handle error
            printMessage("Error accessing directory: " + std::string(e.what()));
        }
    }
    //Execute available program included in present path
    static bool startProgram(std::string program) {
        try {
            if (fs::exists(program) && fs::is_regular_file(program)) {
                //Case if the user is using Windows
                #ifdef _WIN32
                    std::string cmd = "start \"\" \"" + program + "\"";
                #else //Case if the user is using other (Linux, macOS, etc...)
                    std::string cmd = "./" + program;
                #endif
                system(cmd.c_str()); //Execute program
                return true;
            } else { //If the program not found
                printMessage("Program not found: " + program);
                return false;
            }
        } catch (fs::filesystem_error &e) { //Handle error
            printMessage("Error starting program: " + std::string(e.what()));
            return false;
        }
    }
};