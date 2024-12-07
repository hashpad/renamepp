#include "Context.h"

Context::Context()
{
    this->cursorState = CursorState::HIDDEN;
    MenuState* ms = new MenuState();
    UI* ui = new UI(ms);
    this->ui = ui;
}
void Context::init()
{
    initscr();

    this->set_current_option(Option::RENAME);

    std::vector<std::filesystem::path> all_files = helpers::list_dirs_in_path(".");
    this->files = helpers::filter(all_files, { ".jpg", ".png" });

    this->current_file_index = 0;
    while (this->current_file_index < static_cast<int>(files.size())) {
        if (this->current_file_index < 0) {
            this->current_file_index = 0;
            continue;
        }
        const auto& path = this->files[this->current_file_index];

        this->current_file_path = path;
        this->current_filename = path.filename().string();

        this->hook_file();
        this->set_opened_once(false);
        while (this->is_file_hooked()) {
            this->ui->get_gui_state()->show_gui(*this);
        }
    }
}

UI* const Context::get_ui() const
{
    return this->ui;
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
void Context::release_file()
{
    this->file_hooked = false;
}
int Context::get_current_option() const
{
    return this->current_option;
};

void Context::hook_file()
{
    this->file_hooked = true;
}
bool Context::is_file_hooked() const
{
    return this->file_hooked;
}
