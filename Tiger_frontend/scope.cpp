#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <cstdint>
#include <cstring>
#include "antlr4-runtime.h"
#include "tigerParser.h"

#ifndef SCOPE
#define SCOPE

struct Type
{
	int dimension; // array dimension >= 1; if >1 array; if =1 var; -1 no return
	std::string typeVal; // int, float
};

struct Attributes
{
	std::string type; // type, var, func
	Type varType; // for var | type, get more
	Type funcRet;
	std::vector<std::pair<std::string, Type> > funcParams; // <name, Type>, <name, Type>, ...
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

struct RegInfo
{
	RegInfo *caller;
	std::map<std::string, std::pair<int, Type> > defVar;  // defined var or function parameter and their position
	std::map<std::string, std::pair<int, Type> > tempVar; // temp var _t10 and their position
	std::map<std::string, std::pair<int, Type> > outscopeVar; // vars defined outside this scope or outside function
	
	//应该是在这再加一个std::vector<Node> instInfo
	//这个Node如果有inst则我不需要再去funcCode中找了
	std::vector<IRNode *> nodes;
	std::map<std::string, std::vector<Webs> > intWebMap;
    std::map<std::string, std::vector<Webs> > floatWebMap;
    std::map<Webs, std::set<Webs> > intVarGraph;
    std::map<Webs, std::set<Webs> > floatVarGraph;
    std::map<std::string, Webs> varWebMap; // var - its current web
    std::map<std::string, int> spilledVar; 
	Type retVal;
	int spaceSize;
	int curSize;
};

struct Scope
{
	Scope *parent;
	std::map<void *, Scope *> kids; // let scopes
	RegInfo *curReg;
	// <id, attr>
	std::map<std::string, Attributes> table;
	// useful in code generation
	std::vector<std::string> mainCode;
	std::vector<std::string> funcCode;
	std::map<std::string, RegInfo> regTable; // <funcName, RegInfo>
	std::vector<Scope *> scopeKids;
	int whichKid; // the pos in scopeKids of its parent scope
	std::pair<bool, std::string> loopLabel;
	std::pair<bool, std::string> withinFunc;
	Scope operator=(Scope &s)
    {
        curReg = new RegInfo; // 只有curReg改变了
        std::memcpy(curReg, s.curReg, sizeof(RegInfo));
        return *this;
    };
};



#endif