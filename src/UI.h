#pragma once
#include "States.h"
#include <iostream>
#include <memory>
class GUIState;
class UI {
private:
    int text_length;
    std::unique_ptr<GUIState> gui_state;

public:
    UI(GUIState* gui_state);
    UI(std::unique_ptr<GUIState> gui_state);
    void init();
    void detroy();

    void append(const std::string& txt);
    std::string prompt(const std::string& prompt);
    void show_menu(int highlighted_option);

    void clear_screen();

    GUIState* const get_gui_state() const;
    void set_gui_state(GUIState* gui_state);
    ~UI()
    {
    }
};
