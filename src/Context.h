#pragma once

#include "States.h"
#include "UI.h"
#include <filesystem>
#include <ncurses.h>
#include <string>
#include <vector>

class Context {
private:
    UI ui;
    bool running;

    std::string current_filename;
    std::filesystem::path current_file_path;
    std::vector<std::filesystem::path> files;
    int current_file_index;
    bool opened_once;
    int current_option;
    bool file_hooked;

    CursorState cursorState;

public:
    Context(const std::filesystem::path& path, const std::vector<std::string>& file_types);

    UI& get_ui();

    void stop();

    const std::string& get_current_filename() const;
    void set_current_filename(const std::string& name);

    const std::filesystem::path& get_current_file_path() const;
    void set_current_file_path(const std::filesystem::path& path);

    const std::vector<std::filesystem::path>& get_files() const;
    void set_file_path_at_index(int index, const std::filesystem::path& path);

    int get_current_file_index() const;
    void inc_current_file_index();
    void dec_current_file_index();

    bool get_opened_once() const;

    void set_opened_once(bool value);

    int get_current_option() const;
    void set_current_option(int value);
    int get_next_option() const;
    int get_prev_option() const;

    void release_file();
    void hook_file();
    bool is_file_hooked() const;
    ~Context()
    {
        endwin();
    };
};
