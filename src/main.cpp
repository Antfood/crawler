#define DOCTEST_CONFIG_IMPLEMENT
#include "../include/crawler.hpp"
#include "../include/validator.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

int test()
{
 doctest::Context ctx;
 ctx.setOption("abort-after", 5);
 ctx.setOption("no-breaks", true);

 int res = ctx.run();

 if(ctx.shouldExit())
     return res;

  return -1;
}

int main()
{

  int res = test();
 
  ftxui::Element doc = ftxui::hbox
 ({
    ftxui::text("left") | ftxui::border,
    ftxui::text("middle") | ftxui::border | ftxui::flex,
    ftxui::text("right")  | ftxui::border 

  });

  auto screen = ftxui::Screen::Create
 (
    ftxui::Dimension::Full(),
    ftxui::Dimension::Fit(doc)
  );

  ftxui::Render(screen, doc);
  screen.Print();

  return res;
}

