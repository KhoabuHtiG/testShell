#pragma once
#include "../Core.hpp"

static std::string getPermissionString(fs::perms p) {
    auto check = [p](fs::perms bit, char c) {
        return (p & bit) != fs::perms::none ? c : '-';
    };
    return {
        check(fs::perms::owner_read, 'r'),
        check(fs::perms::owner_write, 'w'),
        check(fs::perms::owner_exec, 'x'),
        check(fs::perms::group_read, 'r'),
        check(fs::perms::group_write, 'w'),
        check(fs::perms::group_exec, 'x'),
        check(fs::perms::others_read, 'r'),
        check(fs::perms::others_write, 'w'),
        check(fs::perms::others_exec, 'x')
    };
}

std::string getLastWriteTime(const fs::directory_entry& entry) {
    auto ftime = entry.last_write_time();
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - fs::file_time_type::clock::now()
        + std::chrono::system_clock::now()
    );
    std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

    std::ostringstream out;
    out << std::put_time(std::localtime(&cftime), "%Y-%m-%d %H:%M");
    return out.str();
}
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
static void printLong(const fs::directory_entry& entry) {
    std::string name = entry.path().filename().string();

    fs::perms p = entry.status().permissions();
    auto perms = getPermissionString(p);

    std::string type, sizeStr;
    
    if (entry.is_directory()) {
        sizeStr = std::to_string(getDirectorySize(entry)) + " bytes";
        type = "<DIR>";
    } else if (entry.is_regular_file()) {
        sizeStr = std::to_string(fs::file_size(entry)) + " bytes";
        type = "<FILE>";
    }

    std::ostringstream out;
    out 
        << std::left  << std::setw(12) << perms
        << std::left  << std::setw(20) << getLastWriteTime(entry)
        << std::left  << std::setw(8)  << type
        << std::right << std::setw(12) << sizeStr
        << "  " << name;

    printMessage(out.str());
}


namespace commandType {
    class navigationCommand {
    public:
        static void listItemsInDirectory(const std::string& path, char option) {
            std::ostringstream header;
            header 
                << std::left  << std::setw(12) << "Permission"
                << std::left  << std::setw(20) << "Last Write Time"
                << std::left  << std::setw(8)  << "Type"
                << std::right << std::setw(12) << "Size"
                << "  " << "Name";

            try {
                printMessage("Path: " + path + ":\n");

                switch (option) {
                    case ' ':
                        for (const auto& entry : fs::directory_iterator(path)) {
                            std::string name = entry.path().filename().string();
                            std::string type;

                            if (entry.is_directory()) {
                                type = "<DIR> ";
                            } else if (entry.is_regular_file()) {
                                type = "<FILE> ";
                            }

                            printMessage(type + name);
                        }
                        break;
                    case 'l':
                        printMessage(header.str());
                        printMessage(std::string(10, '-') + "  " + std::string(15, '-') + " " + std::string(8,  '-') + " " + std::string(12, '-') + " " +
                            " " + std::string(30, '-'));

                        for (const auto& entry : fs::directory_iterator(path)) {
                            printLong(entry);
                        }
                        break;
                    default:
                        printMessage("ls: Undefined option");
                        break;
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
        static void showCurrentPath() {printMessage(fs::current_path().string());}
    };
}