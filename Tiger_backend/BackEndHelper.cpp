#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <regex>
#include "BackEndHelper.h"

bool IsOutScopeVar(std::string var, FuncInfo &rinfo){
  if(IsIntNum(var) || IsFloatNum(var)) return false;
  else if(rinfo.varMap.find(var) != rinfo.varMap.end()) return false;
  else if(rinfo.outscopeVarMap.find(var) != rinfo.outscopeVarMap.end()) return false;
  else {
    Type t;
    t.typeVal = "UNKNOWN";
    //std::cout << "insert outscope var " << var << std::endl;
    rinfo.outscopeVarMap.insert(std::make_pair(var, std::make_pair(rinfo.spaceSize,t)));
    rinfo.spaceSize += 4; 
    return true;
  }
}

void CheckOutScopeVar(FuncInfo &rinfo){
  for(auto stmt : rinfo.stmts){
    std::string ir = std::regex_replace(stmt, std::regex("\\s+"), "");
    auto tokens = split(ir, ',');
    if(tokens[0] == "assign") {
          IsOutScopeVar(tokens[1], rinfo);
          IsOutScopeVar(tokens[2], rinfo);
        if(tokens.size() > 3){
          rinfo.arrayVar.insert(std::make_pair(tokens[1], atoi(tokens[2].c_str())));
        }
    } 
    else if (tokens[0]=="array_store" || tokens[0]=="array_load") {
      IsOutScopeVar(tokens[1], rinfo);
      IsOutScopeVar(tokens[2], rinfo);
      IsOutScopeVar(tokens[3], rinfo);
    }
    else if (tokens[0]=="add" || tokens[0]=="sub" || tokens[0]=="mult" || tokens[0]=="div" || tokens[0]=="and" || tokens[0]=="or") {
      IsOutScopeVar(tokens[1], rinfo);
      IsOutScopeVar(tokens[2], rinfo);
      IsOutScopeVar(tokens[3], rinfo);
    } 
    else if (tokens[0]=="breq" || tokens[0]=="brneq" || tokens[0]=="brlt" || tokens[0]=="brgt" || tokens[0]=="brgeq" || tokens[0]=="brleq") {
      IsOutScopeVar(tokens[1], rinfo);
      IsOutScopeVar(tokens[2], rinfo);
    } 
    else if(tokens[0] == "call" || tokens[0] == "callr"){
      int begin;
      if(tokens[0] == "call") begin = 2;
      else begin = 3;
      for(int i = begin; i < tokens.size(); ++i){
        IsOutScopeVar(tokens[i], rinfo);
      }       
    }
    else if(tokens[0] == "return"){
      if(tokens[1] != "") IsOutScopeVar(tokens[1], rinfo);
    }
    else continue;
  }
}

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

bool isFunctionDeclStmt(std::string stmt){
  //return isFunctionCall(stmt);
  if(stmt.find("int")==0 || stmt.find("void")==0 || stmt.find("float")==0){
    return true;
  }
  else return false;
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
	if(opstr=="array_store" || opstr=="array_load"){
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