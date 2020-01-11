#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <fstream>
#include <regex>
#include <iomanip>
#include "scope.cpp"
#include "BackEndHelper.h"

class OptRegisterAllocation {

	int init_loop;
	Scope *globalScope;
	Scope *curScope;
	int totalIntReg = 10;
	int totalFloatReg = 31;
	std::string intRegsName[10] = {"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$t8","$t9"};  
    std::string floatRegsName[31] = {"$f0","$f1","$f2","$f3","$f4","$f5","$f6","$f7","$f8","$f9",
"$f10","$f11","$f12","$f13","$f14","$f15","$f16","$f17","$f18","$f19","$f20","$f21","$f22","$f23",
"$f24","$f25","$f26","$f27","$f28","$f29","$f30"};
    bool floatRegsUsed[31];
    bool intRegsUsed[10];
    std::vector<std::string> freeReservedNT = {"$s0","$s1","$s2"};
    std::vector<std::string> usedReservedNT = {};
public:

	std::vector<std::string> regCode;
	std::map<std::string, std::string> gFloatVal;
	std::string GetReservedNT(){
		if(freeReservedNT.size() > 0){
			std::string reg = freeReservedNT[0];
			usedReservedNT.push_back(reg);
			freeReservedNT.erase(freeReservedNT.begin());
			return reg;
		}		
	}
	void FreeReservedNT(){
		for(auto reg : usedReservedNT){
			freeReservedNT.push_back(reg);
		}
		usedReservedNT.clear();
	}
	void ReleaseDeadVarReg(int line){
		for(auto it = curScope->curReg->varWebMap.begin(); it != curScope->curReg->varWebMap.end(); ){
			bool isDeadVar = true;
			std::string var = it->first;
			for(auto lscope : it->second.liveScopes){
				// 看是不是已经过了最大的lscope.liveEnd
				// free 太激进了
				std::cout << "cur line is " << line << ", var " << var << " liveend is " << lscope.liveEnd << std::endl;
				if(line <= lscope.liveEnd){
					isDeadVar = false;
					break;
				}
			}
			if(isDeadVar){
				std::cout << var << " is dead and will free reg " << it->second.regName << std::endl;
				if(curScope->curReg->defVar.find(var) != curScope->curReg->defVar.end() ||
				curScope->curReg->outscopeVar.find(var) != curScope->curReg->outscopeVar.end()){
					StoreVarRegToMemory(var, it->second.regName);
				}
				FreeWebsReg(it->second);
				curScope->curReg->varWebMap.erase(var);
				it = curScope->curReg->varWebMap.begin();
			}
			else{
				++it;
			}			
		}
	}
	void RegGraphColor(int line){		
		if(curScope->curReg->intWebMap.size() != 0)	GraphColor(curScope->curReg->intVarGraph, curScope->curReg->intWebMap, true, line);
		if(curScope->curReg->floatWebMap.size() != 0) GraphColor(curScope->curReg->floatVarGraph, curScope->curReg->floatWebMap, false, line);
	}
	//friend class CodeGeneration;
	OptRegisterAllocation(Scope *globalscope, std::map<std::string, std::string> &floatVal){
		init_loop = 0;
		globalScope = globalscope;
		curScope = globalscope;
		gFloatVal = floatVal;
		for(int i=0;i<10;++i){
	    	intRegsUsed[i] = false;
	    }
	    for(int i=0;i<31;++i){
	    	floatRegsUsed[i] = false;
	    }
		regCode.push_back(".text");
		regCode.push_back("main:");

		curScope->curReg = &((curScope->regTable.find("main"))->second);
		ReleaseDeadVarReg(-1);
		RegGraphColor(-1);

		/*for(auto it = curScope->curReg->intWebMap.begin();it != curScope->curReg->intWebMap.end();++it){
			std::cout << "var " << it->first << ":";
			for(auto w : it->second){
				std::cout << " < web :" << w.s << ", reg : " << w.regName; 
			}
		}*/
		std::string str = "\taddiu $sp,$sp,-" + std::to_string(curScope->curReg->spaceSize);
		regCode.push_back(str);
		printf("here24\n");
		OptAllocateReg(curScope, curScope->curReg); // 现在ircode是在curReg里面的
		
		//StoreRegToMemory(); 似乎也没必要存回去，中间时才有必要存回去
		str = "\taddiu $sp,$sp," + std::to_string(curScope->curReg->spaceSize);
		regCode.push_back(str);
		regCode.push_back("\tjr $ra");
		for(auto it = regCode.begin(); it != regCode.end(); ++it){
			std::cout << (*it) << std::endl;
		}
	}
	std::string OptAllocateReg(Scope *scope, RegInfo *rinfo);
	void StoreRegToMemory(){
		for(auto it = curScope->curReg->varWebMap.begin(); it != curScope->curReg->varWebMap.end(); ++it){			
			std::string var = it->first;
			if(curScope->curReg->defVar.find(var) == curScope->curReg->defVar.end() &&
				curScope->curReg->outscopeVar.find(var) == curScope->curReg->outscopeVar.end()){
				// means temp register or other const
				continue;
			}
			StoreVarRegToMemory(var, it->second.regName);
		}
	}
	void StoreVarRegToMemory(std::string var, std::string reg){
		int pos = RecurFindVarStackPos(var);
		std::stringstream ss;
		if(reg.at(1)=='f'){
			ss << "\ts.s " << reg << "," << pos << "($sp)";
			regCode.push_back(ss.str());
		}
		else{
			ss << "\tsw " << reg << "," << pos << "($sp)";
			regCode.push_back(ss.str());
		}
	}

	int RecurFindVarStackPos(std::string var){
		int pos = 0;
		Scope locals = *curScope;
		Scope *s = &locals;
		while(s != NULL){
			while(s->curReg != NULL){
				if(s->curReg->defVar.find(var) != s->curReg->defVar.end()){
					pos += ((s->curReg->defVar.find(var))->second).first;
					return pos;
				}
				pos += s->curReg->spaceSize;
				s->curReg = s->curReg->caller;
			}
			s = s->parent;
			s->curReg = &((s->regTable.find("main"))->second);
		}
		return -1;
	}
	void FreeWebsReg(Webs s){
		std::string reg = s.regName;
		FreeReg(reg);
	}
	void FreeReg(std::string reg){
		if(reg.at(1) == 'f'){
			int regIdx = std::stoi(reg.substr(2, reg.length()-2));
			floatRegsUsed[regIdx] = false;
		}
		else{
			int regIdx = std::stoi(reg.substr(2, reg.length()-2));
			intRegsUsed[regIdx] = false;
		}
	}
	void AllocateReg(std::string reg){
		if(reg.at(1) == 'f'){
			int regIdx = std::stoi(reg.substr(2, reg.length()-2));
			floatRegsUsed[regIdx] = true;
		}
		else{
			int regIdx = std::stoi(reg.substr(2, reg.length()-2));
			intRegsUsed[regIdx] = true;
		}
	}

	void GraphColor(std::map<Webs, std::set<Webs> > &map, std::map<std::string, std::vector<Webs>> &varWebMap, bool isIntMap, int lineNum) {
    // free dead var in previous scope or caller, and begin new graph analysis
    // now we still in previous curscope curreg
	    for(auto it = curScope->curReg->varWebMap.begin(); it !=curScope->curReg->varWebMap.end(); ++it){
	      Webs w = it->second;
	      bool varLive = false;
	      for(auto lscope : w.liveScopes){
	        if(lineNum >= lscope.liveBegin && lineNum <= lscope.liveEnd){
	          varLive = true;
	          break;
	        }
	      }
	      if(!varLive){
	        curScope->curReg->varWebMap.erase(it);
	        FreeWebsReg(w);
	      }
	    }
	    // all available reg for this graph coloring
	    std::vector<std::pair<int, bool> > regVec;
	    if(isIntMap){
	      for(int i=0;i<totalIntReg;++i){
	        // means this reg is available
	        if(!intRegsUsed[i]) regVec.push_back(std::make_pair(i, false));
	      }
	    }
	    if(!isIntMap){
	      for(int i=0;i<totalFloatReg;++i){
	        if(!floatRegsUsed[i]) regVec.push_back(std::make_pair(i, false));
	      }
	    }
	    int regNum = regVec.size();

	    std::map<Webs, int> colorMap;
	    for(auto it = map.begin(); it != map.end(); ++it){
	      colorMap[it->first] = -1;
	    }
	    // copy graph
	    std::map<Webs, std::set<Webs> > copyMap;
	    for (auto it = map.begin(); it != map.end(); it++) {
	      std::set<Webs> copySet;
	      for (auto web : it->second) {
	        copySet.insert(web);
	      }
	      copyMap[it->first] = copySet;
	    }

	    std::vector<Webs> stack;
	    while (true) {
	      // While graph G has a node N with degree less than R
	      while (true) {
	        Webs node;
	        bool hasNodeSmaller = false;
	        for(auto it = copyMap.begin(); it != copyMap.end(); ++it){
	          if (it->second.size() < regNum) {
	            node = it->first;
	            hasNodeSmaller = true;
	            break;
	          }
	        }
	        if (hasNodeSmaller == false) break;
	        // push N on a stack S
	        stack.push_back(node);
	        // Remove N and its associated edges from G 
	        copyMap.erase(node);
	        for(auto it = copyMap.begin(); it != copyMap.end(); ++it){
	          if (it->second.find(node) != it->second.end()) {
	            it->second.erase(node);
	          }
	        }
	      }
	      //If the entire graph has been removed then the graph is R-colorable 
	      if (copyMap.size() == 0) {
	        while (!(stack.size() == 0)) {
	          Webs top = stack.back();
	          stack.pop_back();
	          // since every node has its 
	          for (int i = 0; i < regNum; i++) {
	            regVec[i].second = false;
	          }
	          for (Webs neighbor : map[top]) {
	            if (colorMap[neighbor] >= 0) {
	              regVec[colorMap[neighbor]].second = true;
	            }
	          }
	          for (int i = 0; i < regNum; i++) {
	            // if regs[i] is false, then it can be used
	            if (!regVec[i].second) {
	              colorMap[top] = regVec[i].first;
	              break;
	            }
	          }
	        }
	        // done with coloring
	        break;
	      } 
	      else {
	        // 选取第一个spill。但是实际上应该选取spill cost最大的那个
	        // spill时要加上sw和free reg
	        Webs toBeSpilled = copyMap.begin()->first;
	        std::string var;
	        for(auto it = varWebMap.begin(); it != varWebMap.end(); ++it){
	        	auto webs = it->second;
	        	for(auto w : webs){
	        		if(w == toBeSpilled){
	        			var = it->first;
	        			goto SW;
	        		}
	        	}
	        }
	        SW:
	        printf("here1\n");
	        int pos = RecurFindVarStackPos(var);
	        curScope->curReg->spilledVar.insert(std::make_pair(var, pos));
	        printf("here2\n");
	        // Simplify the graph G by choosing an object to spill and remove its node N from G
	        copyMap.erase(toBeSpilled);
	        for(auto it = copyMap.begin(); it != copyMap.end(); ++it){
	          it->second.erase(toBeSpilled);
	        }
	      }
	    }
	    printf("here22\n");
	    // connect web with register
	    for(auto it = varWebMap.begin(); it != varWebMap.end(); ++it){
	      for(auto iter = it->second.begin(); iter != it->second.end(); ++iter){
	        auto itera = (colorMap.find(*iter));
	        if(itera != colorMap.end()){
	        	int regIndex = itera->second;
	        	if(isIntMap) iter->regName = intRegsName[regIndex];
	        	else iter->regName = floatRegsName[regIndex];
	        }
	        else{
	        	std::cout << "can't find web " << iter->s << std::endl;
	        }
	      }
	    }
	    printf("here23\n");
  	}
	bool FindVarRegister(std::string var, int line, std::string &reg);
	void LoadValFromMemorytoReg(std::string var, std::string reg);
	void ArithmeticInstAllocate(std::string ir, int line);
	void AssignInstAllocate(std::string ir, int line);
	void StandLibInstAllocate(std::vector<std::string> &components, int line);
	/*void StoreBackOuterVar(){
		std::stringstream ss;
		for(auto it = curScope->curReg->outscopeVar.begin(); it != curScope->curReg->outscopeVar.end(); ++it){
			Type t = (it->second).second;				
			int pos = (it->second).first;
			int outScopePos = outscopeVarFind(it->first);
			if(t.typeVal == "float"){
				std::string reg = NaiveAllocateFTReg();
				ss << "\tl.s " << reg << "," << pos << "($sp)";
				regCode.push_back(ss.str());
				ss.str("");
				ss << "\ts.s " << reg << "," << outScopePos << "($sp)";
				regCode.push_back(ss.str());
			}
			else if(t.typeVal == "int"){
				std::string reg = NaiveAllocateNTReg();
				ss << "\tlw " << reg << "," << pos << "($sp)";
				regCode.push_back(ss.str());
				ss.str("");
				ss << "\tsw " << reg << "," << outScopePos << "($sp)";
				regCode.push_back(ss.str());
			}
		}
	}*/
};