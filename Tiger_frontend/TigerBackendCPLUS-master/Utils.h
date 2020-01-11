#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// split is to split a string with given token as its name implies
extern std::vector<std::string> split(const std::string &s, char seperator);
extern std::string trim(std::string &str);

#endif