#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <vector>
#include <cstring>

#define LIBRARY_DIR "02_LIBRARY"

enum load_selection {none, files, directories, both};

class Directory
{
private:
    struct Private;
    DIR                        *m_dp;
    struct dirent              *m_dirp;
    std::vector<std::string>    m_files;
    std::vector<std::string>    m_subdirectories;
    std::string                 m_path;

public:
    Directory(); // will open user dir
    explicit Directory(const char* path);
    Directory(Directory &lhs) = default;
    ~Directory();

    void                        load_files();
    std::string                &get_path();
    std::vector<std::string>   &get_files();
    std::vector<std::string>   &get_subdirectories();
    size_t                      subdirectories_count();
    std::string                 get_dirname();

    void                       print_files();
    void                       print_directories();

    static std::string          get_home_path();

};

#endif
