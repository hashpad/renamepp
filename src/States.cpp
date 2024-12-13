#include "States.h"
#include "Context.h"
#include "helpers.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ncurses.h>

void MenuState::script(Context& ctx)
{
    this->show_gui(ctx);

    if (!ctx.get_opened_once()) {
        helpers::open_file(ctx.get_current_file_path());
        ctx.set_opened_once(true);
    }

    this->handle_events(ctx);
}
void MenuState::show_gui(Context& ctx)
{
    UI& ui = ctx.get_ui();
    ui.clear_screen();
    ui.append("Current file is: " + ctx.get_current_filename());
    ui.show_menu(ctx.get_current_option());
}
void MenuState::handle_events(Context& ctx)
{
    UI& ui = ctx.get_ui();

    flushinp();
    int ch = getch();

    switch (ch) {
    case 'j':
        ctx.set_current_option(ctx.get_next_option());
        break;
    case 'k':
        ctx.set_current_option(ctx.get_prev_option());
        break;
    case 'q':
        ctx.stop();
        break;
    case 10:
        switch (ctx.get_current_option()) {
        case Option::RENAME:
            ui.set_gui_state(std::make_unique<RenameState>());
            break;
        case Option::OPEN:
            helpers::open_file(ctx.get_current_file_path());
            break;
        case Option::NEXT:
            ctx.inc_current_file_index();
            ctx.release_file();
            break;
        case Option::PREV:
            ctx.dec_current_file_index();
            ctx.release_file();
            break;
        }
        break;
    }
}

void RenameState::show_gui(Context& ctx)
{
    UI& ui = ctx.get_ui();
    ui.clear_screen();
    this->name = "";
    this->name = ui.prompt("Enter a new name for the file: ");
}
void RenameState::script(Context& ctx)
{
    this->show_gui(ctx);

    if (!this->name.empty()) {
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
    }

    ctx.set_current_option(Option::NEXT);
    UI& ui = ctx.get_ui();
    ui.set_gui_state(std::make_unique<MenuState>());
}
void RenameState::handle_events(Context& ctx)
{
}
