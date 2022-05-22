#include "../include/warning.hpp"

Warning::Warning(const std::string &message, warning_type type)
{
  std::cout << "\n";
  Element el = vbox({ text("    " + message + "    ")}) | borderDouble | center | color(select_color(type));
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(el));
  Render(screen, el);
  screen.Print();
}

Color Warning::select_color (warning_type type)
{
  switch(type)
    {
      case error:
        return Color::Red;
      case success:
        return Color::Green;
      default:
        return Color::White;
    };
}
