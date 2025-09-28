#pragma once
#include "../Core.hpp"

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
    class navigationCommand {
    public:
        static void listItemsInDirectory(const std::string& path, char option) {
            try {
                printMessage("\nDirectory: " + path + "\n");

                if (!fs::is_directory(path)) {
                    printMessage("ls: The '" + path + "' is not recognized as a directory");
                    return;
                }

                for (const auto& entry : fs::directory_iterator(path)) {
                    std::string name = entry.path().filename().string();
                    if (option == ' ') {
                        if (entry.is_directory()) {
                            printMessage("|| <DIR>   " + name);
                        } else if (entry.is_regular_file()) {
                            printMessage("||         " + name);
                        } else {
                            printMessage("|| <OTHER> " + name);
                        }
                    } else if (option == 's') {
                        if (entry.is_directory()) {
                            uintmax_t size = getDirectorySize(entry);
                            printMessage("|| <DIR>   " + name + " | Size: " + std::to_string(size) + " bytes");
                        } else if (entry.is_regular_file()) {
                            uintmax_t size = fs::file_size(entry);
                            printMessage("||         " + name + " | Size: " + std::to_string(size) + " bytes");
                        } else {
                            printMessage("|| <OTHER> " + name);
                        }
                    }
                    else {
                        printMessage("ls: Undefined option -" + std::string(1, option));
                        break;
                    }
                }
            } catch (const fs::filesystem_error& e) {
                printMessage("ls: Error while trying to access directory: " + std::string(e.what()));
            }
        }
        static bool accessDirectory(const std::string path) {
            try {
                if (fs::exists(path) && fs::is_directory(path)) {
                    fs::current_path(path);
                    return true;
                } else {
                    printMessage("cp: The '" + path + "' is not recognized as a path, a file or a directory.");
                    return false;
                }
            } catch (const fs::filesystem_error& e) {
                printMessage("cp: Error while trying to access directory: " + std::string(e.what()));
                return false;
            }
        }
        static void returnToParentDirectory() {
            fs::current_path(fs::current_path().parent_path());
            return;
        };
        static void executeProgram(const std::string program) {
            try {
                if (fs::exists(program) && fs::is_regular_file(program)) {
                    #ifdef _WIN32
                        std::string cmd = "start \"\" \"" + program + "\"";
                    #else
                        std::string cmd = "./" + program;
                    #endif
                    system(cmd.c_str());
                } else {
                    printMessage("exec: The '" + program + "' is not recognized as a possible executed file.");
                }
            } catch (fs::filesystem_error &e) {
                printMessage("exec: Error while trying to start program: " + std::string(e.what()));
            }
            return;
        }
        static void tree(const fs::path path) {
            fs::path root = path.empty() ? fs::current_path() : fs::path(path);

            if (!fs::exists(root)) {
                std::cout << "tree: Path not found: '" << root << "'\n";
                return;
            }

            std::cout << root.string() << "\n";
            for (const auto& entry : fs::directory_iterator(root)) printTree(entry.path(), 1);
        }
        static void showCurrentPath() {
            printMessage(fs::current_path().string());
        }
    };
}