#ifndef USER_INPUT_COMPONENT_H
#define USER_INPUT_COMPONENT_H

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref
#include "../include/validator.hpp"

 using namespace ftxui;

class UserInput
{
  private:
     
     std::string m_output;
     struct Private;

  public:
    UserInput(Changeset &changeset, const std::string &filename, const std::string &path, std::string &ouput);

};


#endif
