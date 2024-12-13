#pragma once

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>

#include <iterator>
#include <ncurses.h>
#include <string>
#include <vector>

namespace helpers {

inline int open_file(const std::string& filepath)
{
    std::string command = "xdg-open '" + filepath + "'";
    int return_code = std::system(command.c_str());
    if (return_code != 0)
        std::cerr << "Error: could not open file with xdg-open" << std::endl;
    return return_code;
}
inline std::vector<std::filesystem::path> list_dirs_in_path(const std::filesystem::path& path)
{
    std::vector<std::filesystem::path> paths;
    if (std::filesystem::exists(path))
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            paths.push_back(entry);
        }
    else {
        std::cerr << "Invalid directory: " << path << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return paths;
}

inline std::vector<std::filesystem::path> filter_files_by_exts(std::vector<std::filesystem::path> paths, std::vector<std::string> exts)
{
    std::vector<std::filesystem::path> filtered_exts;
    std::copy_if(paths.begin(), paths.end(), std::back_inserter(filtered_exts), [&](const std::filesystem::path& path) {
        for (const auto& ext : exts) {
            if (path.extension().string() == "." + ext)
                return true;
        }
        return false;
    });
    return filtered_exts;
}
inline std::string replace_substring(const std::string& str, const std::string& to_replace, const std::string& replace_by)
{
    std::string result(str);
    int pos = 0;
    while ((pos = str.find(to_replace, pos)) != std::string::npos) {
        result.replace(pos, to_replace.length(), replace_by);
        pos += to_replace.length();
    }
    return result;
}
}
