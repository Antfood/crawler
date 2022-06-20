cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# --- Fetch FTXUI --------------------------------------------------------------

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  # Important: Specify a GIT_TAG XXXXX here.
  )

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

project(crawler LANGUAGES CXX VERSION 0.1)

 SET(CMAKE_CXX_FLAGS  "-mmacosx-version-min=10.7 -stdlib=libc++")

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(crawler ${SRC_FILES})

target_include_directories(crawler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(crawler PRIVATE src)

target_link_libraries(crawler
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component 
  )

