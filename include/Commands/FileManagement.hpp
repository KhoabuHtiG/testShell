#pragma once
#include "../Core.hpp"

namespace commandType {
    class fileManamentCommand {
    public:    
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
        static void makeFile(const std::string name) {
            try {
                if (fs::exists(name)) {
                    printMessage("File already existed");
                }

                std::ofstream file(name);
            } catch (const fs::filesystem_error& e) {
                std::string error = e.what();
                printMessage("Error: " + error);
            }
        }
        static void makeDirectory(const std::string direcName) {
            try {
                if (fs::exists(direcName)) {
                    printMessage("Directory already existed");
                    return;
                }

                fs::create_directory(direcName);
            } catch (const fs::filesystem_error& e) {
                std::string error = e.what();
                printMessage("Error: " + error);
            }
        }
        static void typeLineInFile(const std::string input, const std::string file) {
            try {
                if (fs::exists(file)) {
                    printMessage("No such file existed with this name.");
                    return;
                }

                std::ofstream inFile(file, std::ios::app);

                if (inFile.is_open()) {
                    inFile << input << '\n';
                }
                inFile.close();
                return;
            } catch (const fs::filesystem_error& e) {
                std::string error = e.what();
                printMessage("Error: " + error);
            }
        }
    };
}