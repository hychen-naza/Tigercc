#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <regex>
#include "scope.cpp"
#include "BackEndHelper.h"

std::vector<std::string> split(const std::string &s, char seperator) {
  std::vector<std::string> output;
  std::istringstream str(s);
  std::string tmp;
  while (std::getline(str, tmp, seperator)) {
    output.push_back(tmp);
  }
  return output;
}

bool isBinaryInstruction(std::string stmt){
	auto components = split(stmt, ',');
	std::string opstr = components[0];
	if(opstr=="add" || opstr=="sub" || opstr=="mult" || opstr=="div" || opstr=="and" || opstr=="or" || opstr=="assign"){
		return true;
	}
	else return false;
}
bool isMemoryInstruction(std::string stmt){
	auto components = split(stmt, ',');
	std::string opstr = components[0];
	if(opstr=="store" || opstr=="load"){
		return true;
	}
	else return false;
}
bool isBranchInstruction(std::string stmt){
	auto components = split(stmt, ',');
	std::string opstr = components[0];
	if(opstr=="breq" || opstr=="brneq" || opstr=="brlt" || opstr=="brgt" || opstr=="brgeq" || opstr=="brleq"){
		return true;
	}
	else return false;
}
bool isFunctionCall(std::string stmt){
	std::regex fty("(int|void|float)\\s\\w");
	if(std::regex_search(stmt, fty) == 1) return true;
	else return false;
}
bool IsIntNum(const std::string &str){  
    for (int i = 0; i < str.size(); i++){
        int tmp = (int)str[i];
        if (tmp >= 48 && tmp <= 57) continue;
        else return false;
    } 
    return true;
}
bool IsFloatNum(const std::string &str){  
    bool floatPoint = false;
    for (int i = 0; i < str.size(); i++){
        if(str[i]=='.' && floatPoint == false){
            floatPoint = true;
            continue;
        }
        else if(str[i]=='.' && floatPoint == true){
            return false;
        }
        int tmp = (int)str[i];
        if (tmp >= 48 && tmp <= 57) continue;
        else return false;
    } 
    if(floatPoint) return true;
}