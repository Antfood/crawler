#include "../include/directory.hpp"

struct Directory::Private {

  static void open_directory(Directory &self)
  {
    self.m_dp = opendir(self.m_path.c_str());

    if(!self.m_dp)
    {
      std::string msg = "Error: open dir: ";
      msg += strerror(errno);
      throw std::runtime_error(msg.c_str());
    }
  };

  static void load_directory_content(Directory &self, const load_selection selection)
  {
    while((self.m_dirp = readdir(self.m_dp))!= nullptr)
    {
      /* ignore invisible files current dir  and prv dir */
      if(self.m_dirp->d_name[0] == '.' || (strcmp(self.m_dirp->d_name, "..") == 0))
        continue;

      if(is_dir_selected(selection) && self.m_dirp->d_type == DT_DIR)
        self.m_subdirectories.push_back(Private::concat_path(self, self.m_dirp->d_name));

      if(is_file_selected(selection) && self.m_dirp->d_type == DT_REG)
        self.m_files.emplace_back(std::string(self.m_dirp->d_name));
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

/* default constructor opens the user home directory */
Directory::Directory() :
m_path(get_home_path()),
m_dp(nullptr),
m_dirp(nullptr)
{
  Private::open_directory(*this);
  Private::load_directory_content(*this, directories);
}

Directory::Directory(const char *path)
  : m_path(std::string(path)),
    m_dp(nullptr),
    m_dirp(nullptr)
{
  Private::open_directory(*this);
  Private::load_directory_content(*this, directories);

}

Directory::~Directory()
{
  closedir(m_dp);
}

void Directory::load_files()
{

  m_dp = opendir(m_path.c_str());
  Private::load_directory_content(*this, files);
}

std::string &Directory::get_path()
{
  return m_path;
}

std::string Directory::get_home_path()
{
  struct passwd *pw = getpwuid(getuid());
  return pw->pw_dir;
}

std::vector<std::string> &Directory::get_files()
{
  return m_files;
}

std::vector<std::string> &Directory::get_subdirectories()
{
  return m_subdirectories;
}

size_t Directory::subdirectories_count()
{
  return m_subdirectories.size();
}

std::string Directory::get_dirname()
{
  if(m_path == ".")
    return ".";

  std::size_t slash_position = m_path.find_last_of('/');
  return m_path.substr(slash_position + 1, m_path.length());
}

std::string Directory::append_to_path(const std::string &to_append)
{
  return Private::concat_path (*this, to_append.c_str());
};






