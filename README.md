#Crawler


## Installing fxui

Location of the FTXUI installation:
- Headers     : /usr/include/ftxui/
- Libraries   : /usr/lib/ftxui/
- Cmake config: /usr/lib/cmake/ftxui/

The libraries are:
- libscreen.a
- libdom.a
- libcomponent.a
Don't forget to link against them.

They corresponds to the cmake targets:
- ftxui::screen
- ftxui::dom
- ftxui::component
