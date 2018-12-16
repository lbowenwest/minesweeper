#include <vector>
#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include "Grid.h"
#include "Square.h"
#include "MineGrid.h"

int main(int argc, char* argv[]) {

    auto app = Gtk::Application::create(argc, argv, "org.tweakyllama.minesweeper");
    Gtk::Window window;

    MineGrid vis{};

    window.add(vis);
    window.show_all_children();

    return app->run(window);
}
