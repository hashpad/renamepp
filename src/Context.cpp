#include "Context.h"
#include "States.h"
#include "helpers.h"
#include <memory>

Context::Context(const std::filesystem::path& path, const std::vector<std::string>& file_types)
    : ui(UI(std::make_unique<MenuState>()))
    , running(true)

{
    this->files = helpers::list_dirs_in_path(path);
    if (file_types.size() != 0) {
        this->files = helpers::filter_files_by_exts(this->files, file_types);
    }

    if (this->files.size() == 0) {
        std::cerr << "Error: No files were found" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    this->cursorState = CursorState::HIDDEN;
    initscr();

    this->current_file_index = 0;
    while (this->running && this->current_file_index < static_cast<int>(files.size())) {
        if (this->current_file_index < 0) {
            this->current_file_index = 0;
            continue;
        }
        const auto& path = this->files[this->current_file_index];

        this->current_file_path = path;
        this->current_filename = path.filename().string();

        this->hook_file();
        this->set_opened_once(false);
        this->set_current_option(Option::RENAME);
        while (this->is_file_hooked()) {
            this->ui.get_gui_state().script(*this);
        }
    }
}

UI& Context::get_ui()
{
    return ui;
}
void Context::stop()
{
    this->release_file();
    this->running = false;
}

const std::string& Context::get_current_filename() const
{
    return this->current_filename;
};
void Context::set_current_filename(const std::string& name)
{
    this->current_filename = name;
}

const std::filesystem::path& Context::get_current_file_path() const
{
    return this->current_file_path;
}
void Context::set_current_file_path(const std::filesystem::path& name)
{
    this->current_file_path = name;
}

const std::vector<std::filesystem::path>& Context::get_files() const
{
    return this->files;
};
void Context::set_file_path_at_index(int index, const std::filesystem::path& path)
{
    this->files.at(index) = path;
}

int Context::get_current_file_index() const
{
    return this->current_file_index;
}
void Context::inc_current_file_index()
{
    this->current_file_index++;
}
void Context::dec_current_file_index()
{
    this->current_file_index--;
}
bool Context::get_opened_once() const
{
    return this->opened_once;
}
void Context::set_opened_once(bool value)
{
    this->opened_once = value;
}

void Context::set_current_option(int value)
{
    this->current_option = value;
}

int Context::get_current_option() const
{
    return this->current_option;
}
int Context::get_next_option() const
{
    return (this->get_current_option() + 1) % OPTIONS_STRING.size();
}
int Context::get_prev_option() const
{
    int next { 0 };
    if (this->get_current_option() - 1 < 0)
        next = OPTIONS_STRING.size() - 1;
    else {
        next = (this->get_current_option() - 1) % OPTIONS_STRING.size();
    }
    return next;
}

void Context::release_file()
{
    this->file_hooked = false;
}

void Context::hook_file()
{
    this->file_hooked = true;
}
bool Context::is_file_hooked() const
{
    return this->file_hooked;
}
