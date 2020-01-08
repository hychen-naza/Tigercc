#include <map>
#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <cstdint>
#include <cstring>

#ifndef SCOPE
#define SCOPE

struct Type
{
	int dimension; // array dimension >= 1; if >1 array; if =1 var; -1 no return
	std::string typeVal; // int, float
};

struct IRNode
{
	int id;
	std::string content;
	std::vector<int> to;
	std::set<std::string> consts;
	std::set<std::string> defs;
	std::set<std::string> uses;
	std::set<std::string> in;
	std::set<std::string> out;	
	IRNode(int id, std::string content)
	: id(id), content(content){
	}
};

struct Livescope
{
  int liveBegin;
  int liveEnd;
  Livescope(int _liveBegin, int _liveEnd)
  :liveBegin(_liveBegin), liveEnd(_liveEnd){    
  }
};

struct Webs
{
  std::string s;
  std::string regName;
  std::vector<Livescope> liveScopes;
  std::set<int> realUses;
  Webs() {}
  Webs(std::string _s)
  : s(_s){
  }
  bool operator < (const Webs &p)const{
	return s < p.s;	}
  bool operator == (const Webs &p)const{
	return s == p.s; }
};

struct FuncInfo
{
	FuncInfo *caller;
	int spaceSize;
	std::map<std::string, std::pair<int, Type> > varMap;
	std::map<std::string, std::pair<int, Type> > outscopeVarMap;
	std::vector<std::string> stmts;
	std::vector<IRNode *> nodes;
	std::map<std::string, std::vector<Webs> > intWebMap;
    std::map<std::string, std::vector<Webs> > floatWebMap;
    std::map<Webs, std::set<Webs> > intVarGraph;
    std::map<Webs, std::set<Webs> > floatVarGraph;
    std::map<std::string, Webs> varWebMap; // var - its current web
    std::map<std::string, int> arrayVar; // arrayvar - its size
    FuncInfo operator=(FuncInfo &s)
    {
        caller = new FuncInfo; 
        std::memcpy(caller, s.caller, sizeof(FuncInfo));
        return *this;
    };
};




#endif