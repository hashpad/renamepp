#pragma once
#include "helpers.h"
#include <map>

class Context;

enum CursorState {
    SHOWN,
    HIDDEN
};
enum Option {
    RENAME,
    OPEN,
    NEXT,
    PREV,
};

const std::map<Option, std::string> OPTIONS_STRING = {
    { RENAME, "rename" },
    { OPEN, "open" },
    { NEXT, "next" },
    { PREV, "prev" },
};

class GUIState {
public:
    virtual void show_gui(Context& ctx) = 0;
    virtual ~GUIState() { };
};

class MenuState : public GUIState {
private:
    int highlight;
    void set_highlight(int highlight);

public:
    void show_gui(Context& ctx);
    ~MenuState() { };
};

class RenameState : public GUIState {
    void show_gui(Context& ctx);
    ~RenameState() { };
};
