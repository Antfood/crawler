#include "../include/crawler.hpp"

struct Crawler::Private
{
  static void print_files(Directory &current_dir)
  {
    current_dir.load_files();

    for(auto &file : current_dir.get_files())
        std::cout << file << std::endl;
  };
};

Crawler::Crawler() : m_rootdir(Directory("."))
{ }
void Crawler::read_directory_tree()
{
  read_directory_tree(m_rootdir);
};

void Crawler::read_directory_tree(Directory &current_dir)
{
  if(current_dir.get_dirname() == LIBRARY_DIR)
    Private::print_files(current_dir);

  if(current_dir.subdirectories_count() == 0)
    return;

  for(auto &dir : current_dir.get_subdirectories())
  {
    Directory next_dir(dir.c_str());
    read_directory_tree(next_dir);
  };
};

