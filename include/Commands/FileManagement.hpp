#pragma once
#include "../Core.hpp"

namespace commandType {
    class fileManagementCommand {
    public:
        static void renameFile(const std::string path, const std::string name) {
            try {
                if (!fs::exists(path)) {
                    printMessage("rn: The '" + path + "' is not recognized as a path, a file or a directory.");
                    return;
                }

                std::string newFileName = name, oldFileName = path;
                if (std::rename(oldFileName.c_str(), newFileName.c_str()) != 0) {
                    printMessage("rn: Failed to rename '" + path + "'");
                } else {
                    printMessage("rn: Renamed file '" + oldFileName + "' to '" + newFileName + "'");
                }
            } catch (fs::filesystem_error& e) {
                printMessage("rn: Error while trying to rename the file or the direcstory: " + std::string(e.what()));
            }
            return;
        }
        static void deleteFile(const std::string path) {
            try {
                if (!fs::exists(path)) {
                    printMessage("del: The '" + path + "' is not recognized as a path, a file or a directory.");
                }

                if (fs::is_regular_file(path)) {
                    fs::remove(path);
                    printMessage("del: Deleted '" + path + "'");
                } else if (fs::is_directory(path)) {
                    fs::remove_all(path);
                    printMessage("del: Deleted '" + path + "'");
                }
            } catch (fs::filesystem_error& e) {
                printMessage("del: Error while trying to delete the file or the directory: " + std::string(e.what()));
            }
            return;
        }
        static void makeFile(const std::string name) {
            try {
                if (fs::exists(name)) {
                    printMessage("mkf: Cannot put the same name as the existed one.");
                }

                std::ofstream file(name);
            } catch (const fs::filesystem_error& e) {
                printMessage("mkf: Error while trying to create the file: " + std::string(e.what()));
            }
        }
        static void makeDirectory(const std::string direcName, const std::string subDirec) {
            try {
                if (fs::exists(direcName)) {
                    if (subDirec.empty()) printMessage("mkdir: Cannot put the same name as the existed one.");
                    fs::create_directories(direcName + "/" + subDirec);
                    return;
                }

                if (subDirec.empty()) {fs::create_directory(direcName); return;}
                    
                fs::create_directories(direcName + "/" + subDirec);
            } catch (const fs::filesystem_error& e) {
                printMessage("mkdir: Error while trying create the directory: " + std::string(e.what())); }
            }
        static void typeLineInFile(const std::string input, const std::string file) {
            try {
                if (!fs::exists(file)) {
                    printMessage("type: The '" + file + "' is not recognized as a path, a file or a directory.");
                    return;
                }

                std::ofstream inFile(file, std::ios::app);

                if (!inFile.is_open()) printMessage("type: Cannot open file: '" + file + "'");

                inFile << input << '\n';

                inFile.close();
                return;
            } catch (const fs::filesystem_error& e) {
                std::string error = e.what();
                printMessage("type: Error while trying to insert content into file: " + error);
            }
        }    
        static void readFileContent(const std::string file) {
            if (!fs::exists(file)) {
                printMessage("rd: The '" + file + "' is not recognized as a path, a file or a directory.");
                return; 
            }

            if (!fs::is_regular_file(file)) {
                printMessage("rd: The '" + file +"' is not regconized as a readable file");
            }

            std::ifstream readFile(file, std::ios::in);
            if (!readFile.is_open()) {
                printMessage("rd: Cannot open file: '" + file + "'");
            }

            printMessage("-----");

            std::string line;
            while (std::getline(readFile, line)) {
                printMessage(line);
            }
        }
        static void moveFile(const std::string& file, const std::string& destination) {
            try {
                if (!fs::exists(file)) {
                    printMessage("mv: The source '" + file + "' does not exist.");
                    return;
                }

                if (fs::is_directory(destination)) {
                    fs::path destPath = fs::path(destination) / fs::path(file).filename();
                    fs::rename(file, destPath);
                    printMessage("mv: Moved '" + file + "' into directory '" + destination + "'");
                } else {
                    fs::rename(file, destination);
                    printMessage("mv: Moved '" + file + "' to '" + destination + "'");
                }
            } catch (const fs::filesystem_error& e) {
                printMessage("mv: Error moving file or directory: " + std::string(e.what()));
            }
        }
        static void duplicateFile(const std::string& file, std::string destination) {
            try {
                if (!fs::exists(file)) {
                    printMessage("dup: The '" + file + "' is not recognized as a path, a file or a directory.");
                    return;
                }

                if (destination.empty()) {
                    destination = file + "_copy";
                    if (fs::exists(destination)) {
                        destination = file + "_copy1";
                    }
                }

                if (fs::is_directory(file)) {
                    fs::copy(file, destination, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
                    printMessage("dup: Duplicated directory '" + file + "' to '" + destination + "'");
                } else if (fs::is_regular_file(file)) {
                    fs::copy(file, destination, fs::copy_options::overwrite_existing);
                    printMessage("dup: Duplicated file '" + file + "' to '" + destination + "'");
                } else {
                    printMessage("dup: The path '" + file + "' is neither a file nor a directory.");
                }
            }
            catch (fs::filesystem_error& e) {
                printMessage("dup: Error while duplicating: " + std::string(e.what()));
            }
        }
        /*static void showFileProperties(const std::string& file, const char option) {
            std::string fileType;

            if (!fs::exists(file)) {
                printMessage("prop: The '" + file + "' is not recognized as a file.");
                return;
            }

            if (fs::is_regular_file(file)) {
                fileType = "Type: Regular file";
            } else if (fs::is_directory(file)) {
                fileType = "Type: Directory";
            } else {
                fileType = "Type: Other";
            }

            switch (option) {
                case ' ':
                    printMessage("File name: " + file);
                    printMessage(fileType);
                    printMessage("Size: " + sizeof(file));
                    printMessage("Path: " + fs::absolute(file).string());
                    break;
                case 'a':
                    break;
                default:
                    printMessage("prop: Undefined option.");
                    break;
            }
        }*/
    };
}