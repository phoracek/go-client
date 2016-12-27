#ifndef FILE_LOADER_H_
#define FILE_LOADER_H_


#include <string>


class FileLoader {
public:
    static std::string load(std::string file);
    static std::string loadData(std::string file);
};


#endif // FILE_LOADER_H_