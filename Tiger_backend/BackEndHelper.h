#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include "scope.h"
bool IsOutScopeVar(std::string var, FuncInfo &rinfo);
void CheckOutScopeVar(FuncInfo &rinfo);
std::vector<std::string> split(const std::string &s, char seperator);
std::string trim(std::string &str);
bool isFunctionDeclStmt(std::string stmt);
bool isBinaryInstruction(std::string stmt);
bool isMemoryInstruction(std::string stmt);
bool isBranchInstruction(std::string stmt);
bool IsIntNum(const std::string &str);
bool IsFloatNum(const std::string &str);
bool isFunctionCall(std::string stmt);