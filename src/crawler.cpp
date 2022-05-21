#include "../include/crawler.hpp"

struct Crawler::Private
  {
    static void print_files(Directory &current_dir)
    {
      current_dir.load_files();

      for(auto &file : current_dir.get_files())
      std::cout << file << std::endl;
    };

  static void validate_files(Crawler &self, Directory &current_dir)
    {
      current_dir.load_files();

      for(auto &file : current_dir.get_files())
      {
        if(std::regex_search(file, std::regex(".*\\.rtf"))) /* skips .rtf files */
            continue;

        Changeset changeset = self.m_validator.split(file);
        self.m_validator.validate_delimiter(changeset);
        self.m_validator.validate_fields_count(changeset);
        self.m_validator.validate_fields(changeset);

        if(!changeset.m_valid)
        {
          std::string output;
          UserInput ui(changeset, file, current_dir.get_path(), output);
        }

      }
    };
  };

Crawler::Crawler() :
  m_validator(Validator(DELIM)),
  m_rootdir(find_root_dir())
{
}

void Crawler::read_directory_tree()
{
  try
  {
    read_directory_tree(m_rootdir);
  }
  catch(std::exception &err)
  {
    std::cout << err.what() << "\n";
  }
}

Directory Crawler::find_root_dir()
{
  Directory home_dir; /* default constructor return home dir */

  for (auto &subdir : home_dir.get_subdirectories())
    if(std::regex_search(subdir.c_str(), std::regex("dropbox*", std::regex_constants::icase)))
    {
       return Directory(subdir.c_str());
    }
  return home_dir;
}

void Crawler::read_directory_tree(Directory &current_dir)
{
  if(current_dir.get_dirname() == LIBRARY_DIR)
    Private::validate_files(*this, current_dir);

  if(current_dir.subdirectories_count() == 0)
    return;

  for(auto &subdir : current_dir.get_subdirectories())
  {
    Directory next_dir(subdir.c_str());
    read_directory_tree(next_dir);
  }
}

