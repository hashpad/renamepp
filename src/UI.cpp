#include "UI.h"
#include "States.h"
#include <ncurses.h>
#include <string>

UI::UI(GUIState* gui_state)
    : gui_state(gui_state)
{
}
void UI::clear_screen()
{
    clear();
    this->text_length = 0;
    curs_set(0);
}
void UI::append(const std::string& txt)
{
    mvprintw(0, this->text_length, "%s", txt.c_str());
    text_length += txt.length();
}
std::string UI::prompt(const std::string& prompt)
{
    this->append(prompt);
    curs_set(1);
    char input_value[512];
    echo();
    mvgetstr(0, text_length, input_value);
    noecho();

    return std::string(input_value);
}

void UI::show_menu(int highlighted_option)
{

    noecho();
    for (int i = 0; i < OPTIONS_STRING.size(); ++i) {
        if (i == highlighted_option) {
            attron(A_REVERSE);
            mvprintw(5 + i, 10, "%s", OPTIONS_STRING.at(Option(i)).c_str());
            attroff(A_REVERSE);
        } else {
            mvprintw(5 + i, 10, "%s", OPTIONS_STRING.at(Option(i)).c_str());
        }
    }
    refresh();
}
GUIState* const UI::get_gui_state() const
{
    return this->gui_state;
}
void UI::set_gui_state(GUIState* gui_state)
{
    this->gui_state = gui_state;
}
