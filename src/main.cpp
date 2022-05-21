#define DOCTEST_CONFIG_IMPLEMENT
#include "../include/crawler.hpp"
#include "../include/validator.hpp"
#include "../include/input_component.hpp"

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
  Crawler crawler;
  crawler.read_directory_tree();

  return 0;
}

