#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include "scope.h"
#include "BackEndHelper.h"

class RegisterAllocation {
	std::vector<std::string> freeNTReg;
	std::vector<std::string> freeFTReg;
	std::vector<std::string> busyNTReg;
	std::vector<std::string> busyFTReg;
	std::map <std::string, FuncInfo> funcMap;
	int init_loop;
public:
	static std::vector<std::string> regCode;
	std::map<std::string, std::string> gFloatVal;
	RegisterAllocation(FuncInfo &rinfo, std::map <std::string, FuncInfo> funcInstMap, std::map<std::string, std::string> &floatVal){
		init_loop = 0;
		funcMap = funcInstMap;
		gFloatVal = floatVal;
		for(int i=0;i<10;++i){
			std::string temReg;
			temReg = "$t" + std::to_string(i);			
			freeNTReg.push_back(temReg);
		}
		for(int i=0;i<32;++i){
			std::string temReg;
			temReg = "$f" + std::to_string(i);		
			freeFTReg.push_back(temReg);
		}
		AllocateReg(rinfo);
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
	void StoreBackOuterVar(FuncInfo *rinfo){
		std::stringstream ss;
		for(auto it = rinfo->outscopeVarMap.begin(); it != rinfo->outscopeVarMap.end(); ++it){
			Type t = (it->second).second;				
			int pos = (it->second).first;
			int outScopePos = OutscopeVarFind(it->first, rinfo, rinfo->spaceSize);
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
	}
	std::string NaiveAllocateNTReg(){
		if(freeNTReg.size() > 0){
			std::string allocatedReg = freeNTReg[0];
			busyNTReg.push_back(allocatedReg);
			freeNTReg.erase(freeNTReg.begin());
			return allocatedReg;
		}
		else{
			printf("Error no available register\n");
			return "Error";
		}
	}
	std::string NaiveAllocateFTReg(){
		if(freeFTReg.size() > 0){
			std::string allocatedReg = freeFTReg[0];
			busyFTReg.push_back(allocatedReg);
			freeFTReg.erase(freeFTReg.begin());
			return allocatedReg;
		}
		else{
			printf("Error no available register\n");
			return "Error";
		}
	}	
	void NaiveFreeAllReg(){
		NaiveFreeNTReg();
		NaiveFreeFTReg();
	}
	void NaiveFreeNTReg(){
		for(auto it = busyNTReg.begin(); it != busyNTReg.end(); ++it){
			freeNTReg.push_back((*it));
		}		
		busyNTReg.clear();
	}
	void NaiveFreeFTReg(){
		for(auto it = busyFTReg.begin(); it != busyFTReg.end(); ++it){
			freeFTReg.push_back((*it));
		}	
		busyNTReg.clear();
	}

	void AllocateReg(FuncInfo &rinfo);
	void NeedStoreInstsAllocate(std::string ir, FuncInfo &rinfo);
	void BranchInstsAllocate(std::string ir, FuncInfo &rinfo);
	void MemoryInstAllocate(std::string ir, FuncInfo &rinfo);
	void StoreInstAllocate(std::vector<std::string> &components, FuncInfo &rinfo);
	void LoadInstAllocate(std::vector<std::string> &components, FuncInfo &rinfo);

	void ArrayInitAssign(std::vector<std::string> &components, FuncInfo &rinfo);
	void IntArrayInitAssign(std::vector<std::string> &components, int pos);
	void FloatArrayInitAssign(std::vector<std::string> &components, int pos);
	void StandLibInstAllocate(std::vector<std::string> &components, FuncInfo &rinfo);

	bool LWStackPosFind(FuncInfo &rinfo, std::string var, std::string &reg, std::pair<int, Type> &varPair);
	int StackPosFind(FuncInfo &rinfo, std::string var);
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
};