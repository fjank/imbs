#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <iostream>
#include <vector>
#include <dirent.h>
#include <algorithm>
#include <string.h>

class ImageManager
{
    public:
        ImageManager(const std::string &d);
        ~ImageManager();
        inline int getCount() { return count; }
        inline int getEnd() { return end; }
        std::string next();
        std::string prev();
    private:
        void sorting(std::vector<std::string>& data);
        int count, end;
        std::vector<std::string> filename;
};

#endif // IMAGEMANAGER_H
