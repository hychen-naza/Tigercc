#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include "scope.cpp"

std::vector<std::string> split(const std::string &s, char seperator);

bool isBinaryInstruction(std::string stmt);
bool isMemoryInstruction(std::string stmt);
bool isBranchInstruction(std::string stmt);
bool IsIntNum(const std::string &str);
bool IsFloatNum(const std::string &str);
bool isFunctionCall(std::string stmt);