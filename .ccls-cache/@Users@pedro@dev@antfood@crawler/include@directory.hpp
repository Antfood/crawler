#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <cstring>


#define LIBRARY_DIR "LIBRARY"

enum load_selection {none, files, directories, both};

class Directory
{
private:
    struct Private;
    DIR                        *dp;
    struct dirent              *dirp;
    std::vector<std::string>    m_files;
    std::vector<std::string>    m_subdirectories;
    std::string                 m_path;

public:
    Directory(const char* path);

    void                        load_files();
    std::string                &get_path();
    std::vector<std::string>   &get_files();
    std::vector<std::string>   &get_subdirectories();
    size_t                      subdirectories_count();
    const std::string           get_dirname();

    void                        print_files();
    void                        print_directories();

};

#endif
