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
    virtual void script(Context& ctx) = 0;
    virtual void show_gui(Context& ctx) = 0;
    virtual void handle_events(Context& ctx) = 0;
    virtual ~GUIState() { };
};

class MenuState : public GUIState {
private:
    int highlight;
    void set_highlight(int highlight);

public:
    virtual void script(Context& ctx) override;
    virtual void show_gui(Context& ctx) override;
    virtual void handle_events(Context& ctx) override;
    ~MenuState() { };
};

class RenameState : public GUIState {
private:
    std::string name;

public:
    virtual void script(Context& ctx) override;
    virtual void show_gui(Context& ctx) override;
    virtual void handle_events(Context& ctx) override;
    ~RenameState() { };
};
