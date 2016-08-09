#include "imagemanager.h"
#include "natural_less.h"
#include <assert.h>

ImageManager::ImageManager(const std::string &d)
{
    DIR *dir;
    try
    {
        dir = opendir(d.c_str());
    } catch (std::exception& e)
    {
        std::cout << "Directory does not exist " << e.what() << std::endl;
    }

    struct dirent *dp;
    while ((dp=readdir(dir)) != NULL) {
        if(strcmp(dp->d_name, "..") != 0  &&  strcmp(dp->d_name, ".") != 0 && dp->d_name[0] != '.'
                && dp->d_name[0] != '~') {
            filename.push_back(std::string(dp->d_name));
        }
    }

	//std::cout <<  filename.size()  << std::endl;
    //assert(filename.size()== 0); 

    sorting(filename);

    count = -1;
    end = filename.size();

}

ImageManager::~ImageManager() {
    filename.clear();
}

void ImageManager::sorting(std::vector<std::string>& data)
{
    std::sort(data.begin(), data.end(), natural_sort);
}

std::string ImageManager::next() {
    (count < end) ? count++ : count = 0;
    return filename[count];
}

std::string ImageManager::prev() {
    (count >= 0) ? count-- : count = end - 1;
    return filename[count];
}
