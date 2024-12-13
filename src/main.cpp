#include "Context.h"
#include <CLI/CLI.hpp>
#include <string>

int main(int argc, char* argv[])
{

    std::string path { "." };
    std::vector<std::string> file_types;

    CLI::App app;

    app.add_option("path", path, "Path to target directory")->capture_default_str();
    app.add_option("--types", file_types, "File types to include (without '.')");

    CLI11_PARSE(app, argc, argv);
    Context ctxt { std::filesystem::path(path), file_types };
    return 0;
}
