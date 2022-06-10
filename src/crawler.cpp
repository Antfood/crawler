#include "../include/crawler.hpp"

struct Crawler::Private {

  static void rename_when_valid(Changeset &changeset, Directory &current_dir,
                                const std::string &path_to_rename) {
    if (changeset.m_valid) {
      std::string old_path = current_dir.append_to_path(path_to_rename);
      std::string new_path = current_dir.append_to_path(changeset.build_path());
      int result = rename(old_path.c_str(), new_path.c_str());

      if (result != 0) {
        std::string msg("Error renaming file. ");
        msg += strerror(errno);
        throw std::runtime_error(msg.c_str());
      }
    }
  };

  static void validate_changeset(Changeset &changeset, const std::string &current_dir) { Validator::validate_delimiter(changeset);
    Validator::validate_fields_count(changeset);
    Validator::validate_fields(changeset);
    changeset.fill_empty_fields();
    Validator::fix_bad_fields_count(changeset);

    if (changeset.m_valid && changeset.m_cleared)
      Warning("Success!", success);

    if (!changeset.m_valid && changeset.m_cleared)
      Warning("Invalid, Please Try Again.", error);

    if (!changeset.m_valid) {
      UserInput ui(changeset, current_dir);

      if (changeset.m_skipped)
        Warning("Skipped.", skipped);

      if (changeset.m_quit || changeset.m_skipped)
        return;

      changeset.clear_errors();
      validate_changeset(changeset, current_dir);
    }
  };

  static void validate_directory_files(Crawler &self, Directory &current_dir) {
    current_dir.load_files();

    for (auto &file : current_dir.get_files()) {
      if (std::regex_search(file,
                            std::regex(".*\\.rtf"))) /* skips .rtf files */
        continue;

      Changeset changeset = self.m_validator.split(file);
      validate_changeset(changeset, current_dir.get_path());
      if (changeset.m_quit) {
        Warning("Goodbye...", quit);
        exit(0);
      }
      rename_when_valid(changeset, current_dir, file);
    }
  }

  static Directory find_dropbox_directory() {
    Directory volume_dir("/Volumes");

    for (auto &dir : volume_dir.get_subdirectories()) {
      Directory subdirectory(dir.c_str());

      for (auto &subdir : subdirectory.get_subdirectories()) {
        if (std::regex_search(
                subdir.c_str(),
                std::regex("dropbox*", std::regex_constants::icase))) {
          Warning("Welcome! Scanning for library audio files...", welcome);
          return Directory(subdir.c_str());
        }
      }
    }

    Warning("Did not find the Dropbox Directory in your computer. Exiting...",
            error);
    exit(1);
  };
};

Crawler::Crawler()
    : m_validator(Validator(DELIM)), m_rootdir(find_root_dir()) {}

void Crawler::run() {
  try {
    read_directory_tree(m_rootdir);
  } catch (std::exception &err) {
    std::cout << err.what() << "\n";
  }
}

Directory Crawler::find_root_dir() {
  Directory home_dir; /* default constructor return home dir */

  for (auto &subdir : home_dir.get_subdirectories())
    if (std::regex_search(
            subdir.c_str(),
            std::regex("dropbox*", std::regex_constants::icase))) {
      Warning("Welcome! Scanning for library audio files...", welcome);
      return Directory(subdir.c_str());
    }

  /* if dropbox is not found in home scan /Volume */
  return Private::find_dropbox_directory();
}

void Crawler::read_directory_tree(Directory &current_dir) {
  if (current_dir.get_dirname() == LIBRARY_DIR)
    Private::validate_directory_files(*this, current_dir);

  if (current_dir.subdirectories_count() == 0)
    return;

  for (auto &subdir : current_dir.get_subdirectories()) {
    Directory next_dir(subdir.c_str());
    read_directory_tree(next_dir);
  }
}
