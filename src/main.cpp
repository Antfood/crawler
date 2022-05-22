#define DOCTEST_CONFIG_IMPLEMENT
#include "../include/crawler.hpp"

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.


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
  crawler.read_directory_tree();


  return 0;
}

