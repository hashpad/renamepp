#pragma once
#include <map>
#include <string>

class Context;

enum CursorState {
    SHOWN,
    HIDDEN
};
enum Option {
    RENAME,
    NEXT,
    PREV,
    OPEN,
};

const std::map<Option, std::string> OPTIONS_STRING = {
    { Option::RENAME, "rename" },
    { Option::OPEN, "open" },
    { Option::NEXT, "next" },
    { Option::PREV, "prev" },
};

class GUIState {
public:
    virtual void script(Context& ctx) = 0;
    virtual void show_gui(Context& ctx) = 0;
    virtual void handle_events(Context& ctx) = 0;
    virtual ~GUIState() { };
};

class MenuState : public GUIState {
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
class DeleteState : public GUIState {
    virtual void script(Context& ctx) override;
    virtual void show_gui(Context& ctx) override;
    virtual void handle_events(Context& ctx) override;
};
