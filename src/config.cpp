#include "config.h"

#include "file_loader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Config::Config(std::string file) {
    std::string temp = FileLoader::load(file);
    std::string line;
    std::istringstream ss(temp);
    while (std::getline(ss, line)) {
        if (line[0] == '#') {
            continue;
        }
        std::istringstream iss(line);
        std::string key, value;
        iss >> key;
        std::getline(iss, value);
        values_[key] = value.substr(1);
    }
}

Config::~Config() {

}


std::string Config::getValue(std::string key) {
    return values_[key];
}
