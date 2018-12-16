#pragma once

#include <SFML/Graphics.hpp>
#include <gtkmm/widget.h>

class SFMLWidget : public Gtk::Widget {
public:
    explicit SFMLWidget(sf::VideoMode mode, int size_request = -1);
    ~SFMLWidget() override = default;

    void invalidate();
    void display();

    sf::RenderWindow renderWindow;

protected:
  void on_size_allocate(Gtk::Allocation &allocation) override;
  void on_realize() override;
  void on_unrealize() override;

  sf::VideoMode video_mode;
  Glib::RefPtr<Gdk::Window> window;
};
