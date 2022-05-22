#ifndef WARNING_H
#define WARNING_H
#include <string>
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/util/ref.hpp"

using namespace ftxui;

enum warning_type {error, success, skipped, quit};

class Warning
{
 public:
  Warning(const std::string &message, warning_type type);
  static Color select_color(warning_type type);
};

#endif