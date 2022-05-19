#include "../include/input_component.hpp"


struct UserInput::Private
{
    static Component create_event(ScreenInteractive &screen, Component &renderer)
    {
      return CatchEvent(renderer, [&](Event event) {
        if (event == Event::Character('\n')) {
          screen.ExitLoopClosure()();
          return true;
        }
        return false;
      });
    }

  };

UserInput::UserInput(Changeset &changeset, const std::string &filename, const std::string &path, std::string &output)
{
  Component user_input         = Input(&output, "New Name: ");
  Component vertical_container = Container::Vertical({ user_input });

  Component renderer = Renderer(vertical_container, [&] { 
    return  vbox({
               text(filename) | color(Color::Blue),
               text(changeset.first_error()) | color(Color::Red),
               separator(),
               text("Found at -> " + path +  ". Press 'o' to open in Finder.") | color(Color::Cyan),
               text("Other Errors Found -> " + changeset.other_errors()) | color(Color::Cyan),
               separator(),
               hbox(text(" Fix Filename: "), user_input->Render()),
           }) | border;
  });

  auto screen                  = ScreenInteractive::TerminalOutput();
  auto event                   = Private::create_event(screen, renderer);

  screen.Loop(event);
};


