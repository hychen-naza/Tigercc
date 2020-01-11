#include <algorithm>

#include "Utils.h"

std::vector<std::string> split(const std::string &s, char seperator) {
  std::vector<std::string> output;
  std::istringstream str(s);
  std::string tmp;
  while (std::getline(str, tmp, seperator)) {
    output.push_back(tmp);
  }
  return output;
}

std::string trim(std::string &str) {
  std::replace(str.begin(), str.end(), '\t', ' ');

  std::size_t first = str.find_first_not_of(' ');
  if (first == std::string::npos) return "";

  std::size_t last = str.find_last_not_of(' ');

  return str.substr(first, last - first + 1);
}