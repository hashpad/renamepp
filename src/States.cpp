#include "Context.h"

void MenuState::script(Context& ctx)
{
    this->show_gui(ctx);

    if (!ctx.get_opened_once()) {
        helpers::open_file(ctx.get_current_file_path());
        ctx.set_opened_once(true);
    }

    this->handle_events(ctx);
};
void MenuState::show_gui(Context& ctx)
{
    UI& ui = ctx.get_ui();
    ui.clear_screen();
    ui.append("Current file is: " + ctx.get_current_filename());
    ui.show_menu(highlight);
}
void MenuState::handle_events(Context& ctx)
{
    UI& ui = ctx.get_ui();
    int ch = getch();

    switch (ch) {
    case 'j':
        highlight = (highlight + 1) % OPTIONS_STRING.size();
        break;
    case 'k':
        if (highlight - 1 < 0)
            highlight = OPTIONS_STRING.size() - 1;
        else {
            highlight = (highlight - 1) % OPTIONS_STRING.size();
        }

        break;
    case 10:
        if (highlight == 0) {
            ui.set_gui_state(new RenameState());
        }
        if (highlight == 1) {
            int return_code = helpers::open_file(ctx.get_current_file_path());
        }
        if (highlight == 2) {
            ctx.inc_current_file_index();
            ctx.release_file();
        }
        if (highlight == 3) {
            ctx.dec_current_file_index();
            ctx.release_file();
        }
        break;
    }
}

void RenameState::show_gui(Context& ctx)
{
    UI& ui = ctx.get_ui();
    ui.clear_screen();
    this->name = ui.prompt("Enter a new name for the file: ");
}
void RenameState::script(Context& ctx)
{
    this->show_gui(ctx);

    this->name = helpers::replace_substring(this->name, " ", "_");
    std::filesystem::path path = ctx.get_current_file_path();
    std::string ext = ctx.get_current_file_path().extension().string();

    std::string new_path = path.parent_path().append(this->name.append(ext));
    std::error_code ec;

    std::filesystem::rename(path, new_path, ec);

    if (!ec) {
        ctx.set_current_filename(this->name);
        ctx.set_current_file_path(new_path);
        ctx.set_file_path_at_index(ctx.get_current_file_index(), new_path);
    } else {
        std::cerr << "Error: " << ec.value() << std::endl;
        exit(ec.value());
    }

    UI& ui = ctx.get_ui();
    ui.set_gui_state(new MenuState());
}

void RenameState::handle_events(Context& ctx)
{
}
