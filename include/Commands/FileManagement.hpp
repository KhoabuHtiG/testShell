#pragma once
#include "../Core.hpp"

namespace commandType {
    class fileManagementCommand {
    public:    
        static void renameFile(const std::string path, const std::string name) {
            try {
                if (!fs::exists(path)) {
                    printMessage("The '" + path + "' is not recognized as a path, a file or a directory.");
                    return;
                }

                std::string newFileName = name, oldFileName = path;
                if (std::rename(oldFileName.c_str(), newFileName.c_str()) != 0) {
                    printMessage("Error while renaming name.");
                } else {
                    printMessage("Renamed file '" + oldFileName + "' to '" + newFileName + "'");
                }
            } catch (fs::filesystem_error& e) {
                printMessage("Error while remaming file: " + std::string(e.what()));
            }
            return;
        }
        static void deleteFile(const std::string path) {
            try {
                if (!fs::exists(path)) {
                    printMessage("No such path found with this name.");
                }

                if (fs::is_regular_file(path)) {
                    fs::remove(path);
                    printMessage("Removed '" + path + "'");
                } else if (fs::is_directory(path)) {
                    fs::remove_all(path);
                    printMessage("Removed '" + path);
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
                if (!fs::exists(file)) {
                    printMessage("No such file existed with this name.");
                    return;
                }

                std::ofstream inFile(file, std::ios::app);

                if (!inFile.is_open()) {
                    printMessage("Cannot open file: '" + file + "'");
                }

                inFile << input << '\n';

                inFile.close();
                return;
            } catch (const fs::filesystem_error& e) {
                std::string error = e.what();
                printMessage("Error: " + error);
            }
        }
        static void readFileContent(const std::string file) {
            if (!fs::exists(file)) {
                printMessage("No such file found with this name.");
                return; 
            }

            std::ifstream readFile(file, std::ios::in);
            if (!readFile.is_open()) {
                printMessage("Cannot open file: '" + file + "'");
            }

            std::string line;
            while (std::getline(readFile, line)) {
                printMessage(line);
            }
        }
    };
}