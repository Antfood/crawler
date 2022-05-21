#ifndef CRAWLER_H
#define CRAWLER_H

#include "../include/directory.hpp"
#include "../include/validator.hpp"
#include "../include/input_component.hpp"

class Crawler
{
    Directory m_rootdir;
    Validator m_validator;
    struct Private;

 public:
    Crawler();
    void read_directory_tree();
    void read_directory_tree(Directory &current_dir);
    static Directory find_root_dir();

};


#endif
