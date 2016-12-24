#include "file_loader.h"

#include <fstream>

std::string FileLoader::load(std::string file) {
    std::ifstream infile{file};
    std::string file_contents{std::istreambuf_iterator<char>(infile),
                              std::istreambuf_iterator<char>()};

    return file_contents;
}

std::string FileLoader::loadData(std::string file) {
    return load("../data/" + file);
}