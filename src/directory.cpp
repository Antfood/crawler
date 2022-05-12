#include "../include/directory.hpp"

struct Directory::Private {

  static void load_directory_content(Directory &self, const load_selection selection)
  {
    while((self.dirp = readdir(self.dp))!= NULL)
      {
        /* ignore invisible filesm current dir  and prv dir */
        if(self.dirp->d_name[0] == '.' || (strcmp(self.dirp->d_name, "..") == 0))
          continue;

        if(is_dir_selected(selection) && self.dirp->d_type == DT_DIR)
          self.m_subdirectories.push_back(Private::concat_path(self, self.dirp->d_name));

        if(is_file_selected(selection) && self.dirp->d_type == DT_REG)
          self.m_files.push_back(std::string(self.dirp->d_name));
      }
  };

  static std::string concat_path(Directory &self, const char *name)
  {
     return self.m_path + "/" + name;
  };

  static bool is_file_selected(const load_selection selection)
  {
    return selection == files || selection == both;
  };

static bool is_dir_selected(const load_selection selection)
  {
    return selection == directories || selection == both;
  };
};

Directory::Directory(const char *path)
: m_path(std::string(path))
{
  dp = opendir(m_path.c_str());
  Private::load_directory_content(*this, directories);

};

void Directory::load_files()
{

    dp = opendir(m_path.c_str());
    Private::load_directory_content(*this, files);
};

std::string &Directory::get_path()
{
      return m_path;
};

std::vector<std::string> &Directory::get_files()
{
  return m_files;
};

std::vector<std::string> &Directory::get_subdirectories()
{
  return m_subdirectories;
};

size_t Directory::subdirectories_count()
{
  return m_subdirectories.size();
};

const std::string Directory::get_dirname()
{
  if(m_path == ".")
      return std::string(".");

   std::size_t slash_position = m_path.find_last_of("/");
   return m_path.substr(slash_position + 1, m_path.length());
};


void Directory::print_files()
{
    for(auto &file : m_files)
       std::cout << file << std::endl;
};

void Directory::print_directories()
{
    for(auto &directory : m_subdirectories)
       std::cout << directory << std::endl;

};



