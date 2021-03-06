#define DOCTEST_CONFIG_IMPLEMENT
#include "../include/crawler.hpp"
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
 // test();
  Crawler crawler;
  crawler.run ();

  return 0;
}

