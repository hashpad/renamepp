#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>

#include <iterator>
#include <ncurses.h>
#include <string>
#include <system_error>
#include <vector>

int text_length = 0;

int open_file(const std::string& filepath)
{
    std::string command = "feh " + filepath;
    int return_code = std::system(command.c_str());
    if (return_code != 0)
        std::cerr << "Error: could not open file with xdg-open" << std::endl;
    return return_code;
}
std::vector<std::filesystem::path> list_dirs_in_path(const std::filesystem::path& path)
{
    std::vector<std::filesystem::path> paths;
    if (std::filesystem::exists(path))
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            paths.push_back(entry);
        }
    else {
        std::cerr << "Invalid directory: " << path << std::endl;
    }
    return paths;
}

void initialize_ncurses()
{
    initscr();
    curs_set(0);
}

void clean_up()
{
    endwin();
}

void clear_screen()
{
    clear();
    text_length = 0;
    curs_set(0);
}
void append(const std::string& txt)
{
    mvprintw(0, text_length, "%s", txt.c_str());
    text_length += txt.length();
}
std::string prompt(const std::string& prompt)
{
    append(prompt);
    curs_set(1);
    char input_value[512];
    echo();
    mvgetstr(0, text_length, input_value);
    noecho();

    return std::string(input_value);
}

const std::vector<std::string> choices = {
    "rename",
    "open",
    "next",
    "prev"
};
void show_menu(int highlight)
{

    noecho();
    for (int i = 0; i < choices.size(); ++i) {
        if (i == highlight) {
            attron(A_REVERSE);
            mvprintw(5 + i, 10, "%s", choices.at(i).c_str());
            attroff(A_REVERSE);
        } else {
            mvprintw(5 + i, 10, "%s", choices.at(i).c_str());
        }
    }
    refresh();
}
enum State {
    RENAME_STATE,
    MENU_STATE,
};
std::vector<std::filesystem::path> filter(std::vector<std::filesystem::path> paths, std::vector<std::string> exts)
{
    std::vector<std::filesystem::path> filtered_exts;
    std::copy_if(paths.begin(), paths.end(), std::back_inserter(filtered_exts), [&](const std::filesystem::path& path) {
        for (const auto& ext : exts) {
            if (path.extension().string() == ext)
                return true;
        }
        return false;
    });
    return filtered_exts;
}
std::string replace_substring(const std::string& str, const std::string& to_replace, const std::string& replace_by)
{
    std::string result(str);
    int pos = 0;
    while ((pos = str.find(to_replace, pos)) != std::string::npos) {
        result.replace(pos, to_replace.length(), replace_by);
        pos += to_replace.length();
    }
    return result;
}
int main(int argc, char* argv[])
{
    initialize_ncurses();

    int highlight = 0;
    bool running = true;
    int ch;
    State state = MENU_STATE;

    std::vector<std::filesystem::path> files = list_dirs_in_path(".");
    files = filter(files, { ".jpg", ".png" });

    int i = 0;
    while (i < static_cast<int>(files.size())) {
        if (i < 0) {
            i = 0;
            continue;
        }
        const auto& path = files[i];

        const std::string ext = path.filename().extension().string();

        bool at_file = true;
        bool opened_once = false;
        std::string current_path = path;
        std::string current_filename = path.filename().string();
        while (at_file) {
            switch (state) {
            case MENU_STATE:
                clear_screen();
                append("Current file is: " + current_filename);

                show_menu(highlight);
                if (!opened_once) {
                    open_file(current_path);
                    opened_once = true;
                }

                ch = getch();

                switch (ch) {
                case 'j':
                    highlight = (highlight + 1) % choices.size();
                    break;
                case 'k':
                    if (highlight - 1 < 0)
                        highlight = choices.size() - 1;
                    else {
                        highlight = (highlight - 1) % choices.size();
                    }

                    break;
                case 10:
                    if (highlight == 0) {
                        state = RENAME_STATE;
                    }
                    if (highlight == 1) {
                        int return_code = open_file(current_path);
                    }
                    if (highlight == 2) {
                        ++i;
                        at_file = false;
                    }
                    if (highlight == 3) {
                        --i;
                        at_file = false;
                    }
                    break;
                }
                break;
            case RENAME_STATE:
                clear_screen();
                std::string new_filename = prompt("Enter a new name for the file: ");
                new_filename = replace_substring(new_filename, " ", "_");

                std::string new_path = path.parent_path().append(new_filename.append(ext));
                std::error_code ec;

                std::filesystem::rename(current_path, new_path, ec);

                if (!ec) {
                    current_filename = new_filename;
                    current_path = new_path;
                    files[i] = current_path;
                } else {
                    std::cerr << "Error: " << ec.value() << std::endl;
                    exit(ec.value());
                }

                state = MENU_STATE;
                break;
            }
        }
    }

    clean_up();
    return 0;
}
