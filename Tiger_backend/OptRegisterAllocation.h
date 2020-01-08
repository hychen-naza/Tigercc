#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <fstream>
#include <regex>
#include <iomanip>
#include "scope.h"
#include "BackEndHelper.h"

class OptRegisterAllocation {

	int init_loop;
	std::vector<std::string> freeReservedNT = {"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$s8"};
    std::vector<std::string> usedReservedNT = {};
    std::vector<std::string> freeReservedFT = {"$f0","$f1","$f2","$f3","$f4"};
    std::vector<std::string> usedReservedFT = {};
	std::map <std::string, FuncInfo> funcMap;
public:

	static std::vector<std::string> regCode;
	std::map<std::string, std::string> gFloatVal;
	OptRegisterAllocation(FuncInfo &rinfo, std::map <std::string, FuncInfo> funcInstMap, std::map<std::string, std::string> &floatVal){
		init_loop = 0;
		gFloatVal = floatVal;
		funcMap = funcInstMap;
		OptAllocateReg(rinfo); 
	}
	void OptAllocateReg(FuncInfo &rinfo);

	// store var 
	// store var 
	// store var 
	void ReleaseAllVarReg(FuncInfo &rinfo){
		for(auto it = rinfo.varWebMap.begin(); it != rinfo.varWebMap.end(); ++it){
			//注意，常数也会出现在varWebMap中
			if(IsIntNum(it->first) || IsFloatNum(it->first)) continue;
			//std::cout << "var is " << it->first << std::endl;
			StoreVarRegToMemory(rinfo, it->first, it->second.regName);			
		}
		rinfo.varWebMap.clear();
	}
	// 用于assign, add等有更新值的指令，都会检查下def var是否要存回去
	void StoreVarToMemory(FuncInfo &rinfo, std::string var, std::string reg, int line){
		auto it = rinfo.varWebMap.find(var);
		if(it != rinfo.varWebMap.end()){
			// spill var肯定要存回去
			if(it->second.regName == "spill"){
				StoreVarRegToMemory(rinfo, var, reg);
				rinfo.varWebMap.erase(it);
			}					
			// dead web 也要存回去，并从varWebMap中删除
			else{
				bool deadWeb = true;
				auto lives = it->second.liveScopes;
				for(auto live : lives){
					if(live.liveEnd > line){
						deadWeb = false;
						break;
					}
				}
				if(deadWeb){
					StoreVarRegToMemory(rinfo, var, reg);
					rinfo.varWebMap.erase(it);
				}
			}			
		}
		else{
			//std::cout << "var " << var << " is not in varWebMap; reg is " << reg << std::endl;
			std::cout << "error in opt.h 92 " << std::endl;
		}
	}
	void StoreVarRegToMemory(FuncInfo &rinfo, std::string var, std::string reg){
		int pos = StackPosFind(rinfo, var);
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
	void StoreBackOuterVar(FuncInfo *rinfo){
		std::stringstream ss;
		for(auto it = rinfo->outscopeVarMap.begin(); it != rinfo->outscopeVarMap.end(); ++it){
			Type t = (it->second).second;				
			int pos = (it->second).first;
			int outScopePos = OutscopeVarFind(it->first, rinfo, rinfo->spaceSize);
			// 总觉得这地方不太对啊，没必要lw,sw
			// 是要用的，outscope的reg全被释放了，所以没有reg可以接这个值，得直接存到内存
			if(t.typeVal == "float"){
				std::string reg = GetReservedFT();
				ss << "\tl.s " << reg << "," << pos << "($sp)";
				regCode.push_back(ss.str());
				ss.str("");
				ss << "\ts.s " << reg << "," << outScopePos << "($sp)";
				regCode.push_back(ss.str());
				FreeReservedFT();
			}
			else if(t.typeVal == "int"){
				std::string reg = GetReservedNT();
				ss << "\tlw " << reg << "," << pos << "($sp)";
				regCode.push_back(ss.str());
				ss.str("");
				ss << "\tsw " << reg << "," << outScopePos << "($sp)";
				regCode.push_back(ss.str());
				FreeReservedNT();	
			}
		}
	}


	// var pos find
	// var pos find
	// var pos find
	int StackPosFind(FuncInfo &rinfo, std::string var){
		auto it1 = rinfo.varMap.find(var);
		if(it1 != rinfo.varMap.end()){
			return it1->second.first;
		}
		auto it2 = rinfo.outscopeVarMap.find(var);
		if(it2 != rinfo.outscopeVarMap.end()){
			return it2->second.first;
		}
		std::cout << "\n\nattention not find var is " << var << std::endl;
		return -1;
	}
	
	int StackPosFind(FuncInfo &rinfo, std::string var, std::pair<int, Type> &varPair){
		auto it1 = rinfo.varMap.find(var);
		if(it1 != rinfo.varMap.end()){
			varPair = it1->second;
			return it1->second.first;
		}
		auto it2 = rinfo.outscopeVarMap.find(var);
		if(it2 != rinfo.outscopeVarMap.end()){
			varPair = it2->second;
			return it2->second.first;
		}
		std::cout << "\n\nattention not find var is " << var << std::endl;
		return -1;
	}
	int OutscopeVarFind(std::string var, FuncInfo *callerRinfo, int curpos){
		int pos = curpos;
		// 即只往外找一层，
		if(callerRinfo->varMap.find(var) != callerRinfo->varMap.end()){
			pos += ((callerRinfo->varMap.find(var))->second).first;
			return pos;
		}		
		else if(callerRinfo->outscopeVarMap.find(var) != callerRinfo->outscopeVarMap.end()){
			pos += ((callerRinfo->outscopeVarMap.find(var))->second).first;
			return pos;
		}
	}


	// load var
	// load var
	// load var
	void LoadValFromMemorytoReg(FuncInfo &rinfo, std::string var, std::string reg){
		int pos = StackPosFind(rinfo, var);
		std::stringstream ss;
		if(reg.at(1) == 'f'){
			ss << "\tl.s " << reg << "," << pos << "($sp)";
		}
		else{
			ss << "\tlw " << reg << "," << pos << "($sp)";
		}
		regCode.push_back(ss.str());
	}
	bool FindVarRegister(FuncInfo &rinfo, std::string var, int line, std::string &reg){
		
		std::stringstream ss;
		bool find = false;
		auto it1 = rinfo.intWebMap.find(var);
		auto it2 = rinfo.floatWebMap.find(var);
		if(it1 != rinfo.intWebMap.end()){
			for(Webs w : it1->second){
				for(Livescope l : w.liveScopes){
					if(line >= l.liveBegin && line <= l.liveEnd){
						find = true;
						if(w.regName == "spill"){
							rinfo.varWebMap[var] = w;
							goto NTRESERVED;
						}
						reg = w.regName;
						if(isalpha(var.at(0)) && rinfo.varWebMap.find(var) == rinfo.varWebMap.end()){
							LoadValFromMemorytoReg(rinfo, var, reg);
						}
						// 每次找到的Var都必须在varWebMap中更新，spill var也加入varWebMap
						rinfo.varWebMap[var] = w;
					}
				}			
			}
		}
		
		if(it2 != rinfo.floatWebMap.end()){
			for(Webs w : it2->second){
				for(Livescope l : w.liveScopes){
					if(line >= l.liveBegin && line <= l.liveEnd){
						find = true;
						if(w.regName == "spill"){
							rinfo.varWebMap[var] = w;
							goto FTRESERVED;
						}
						reg = w.regName;
						if(isalpha(var.at(0)) && rinfo.varWebMap.find(var) == rinfo.varWebMap.end()){
							LoadValFromMemorytoReg(rinfo, var, reg);
						}
						rinfo.varWebMap[var] = w;
					}
				}			
			}
		}
		
		if(IsIntNum(var)){
			ss << "\tli " << reg << "," << var;
			regCode.push_back(ss.str());
		}	
		else if(IsFloatNum(var)){
			std::string floatVar = (gFloatVal.find(var))->second;
			ss << "\tl.s " << reg << "," << floatVar;
			regCode.push_back(ss.str());
		}
		return find;

		NTRESERVED:
		 // 此时还不能free,必须等该指令结束才行
		reg = GetReservedNT();
		if(IsIntNum(var)){
			ss << "\tli " << reg << "," << var;
			regCode.push_back(ss.str());
		}
		else{
			int pos = StackPosFind(rinfo, var);
			ss << "\tlw " << reg << "," << pos << "($sp)";
			regCode.push_back(ss.str());
		}
		return find;
		FTRESERVED:
		reg = GetReservedFT(); 
		if(IsFloatNum(var)){
			std::string floatVar = (gFloatVal.find(var))->second;
			ss << "\tl.s " << reg << "," << floatVar;
			regCode.push_back(ss.str());
		}
		else{
			int pos = StackPosFind(rinfo, var);
			ss << "\tl.s " << reg << "," << pos << "($sp)";
			regCode.push_back(ss.str());
		}
		return find;
	}

	void ArithmeticInstAllocate(FuncInfo &rinfo, std::string ir, int line);
	void BranchInstAllocate(FuncInfo &rinfo, std::string ir, int line);
	void AssignInstAllocate(FuncInfo &rinfo, std::string ir, int line);
	void StandLibInstAllocate(FuncInfo &rinfo, std::vector<std::string> &components, int line);

	void ArrayInitAssign(std::vector<std::string> &components, FuncInfo &rinfo);
	void IntArrayInitAssign(std::vector<std::string> &components, int pos);
	void FloatArrayInitAssign(std::vector<std::string> &components, int pos);

	void MemoryInstAllocate(std::string ir, FuncInfo &rinfo, int line);
	void StoreInstAllocate(std::vector<std::string> &components, FuncInfo &rinfo, int line);
	void LoadInstAllocate(std::vector<std::string> &components, FuncInfo &rinfo, int line);

	std::string GetReservedNT(){
		if(freeReservedNT.size() > 0){
			std::string reg = freeReservedNT[0];
			usedReservedNT.push_back(reg);
			freeReservedNT.erase(freeReservedNT.begin());
			return reg;
		}		
		else{
			return "Error";
		}
	}
	void FreeReservedNT(){
		for(auto reg : usedReservedNT){
			freeReservedNT.push_back(reg);
		}
		usedReservedNT.clear();
	}
	std::string GetReservedFT(){
		if(freeReservedFT.size() > 0){
			std::string reg = freeReservedFT[0];
			usedReservedFT.push_back(reg);
			freeReservedFT.erase(freeReservedFT.begin());
			return reg;
		}	
		else{
			return "Error";
		}	
	}
	void FreeReservedFT(){
		for(auto reg : usedReservedFT){
			freeReservedFT.push_back(reg);
		}
		usedReservedFT.clear();
	}
};


