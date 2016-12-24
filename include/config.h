#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <map>

class Config {
public:
    Config(std::string file);
    ~Config();

    std::string getValue(std::string key);

private:
    std::map<std::string, std::string> values_;
};

#endif // CONFIG_H_