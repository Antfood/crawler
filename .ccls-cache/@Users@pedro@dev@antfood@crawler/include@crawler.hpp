#ifndef CRAWLER_H
#define CRAWLER_H

#include "../include/directory.hpp"

class Crawler
{
    Directory m_rootdir;
    struct Private;

 public:
    Crawler();
    void read_directory_tree();
    void read_directory_tree(Directory &current_dir);

};


#endif
