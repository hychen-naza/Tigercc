#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include "scope.cpp"
#include "BackEndHelper.h"

class RegisterAllocation {
	std::vector<std::string> freeNTReg;
	std::vector<std::string> freeFTReg;
	std::vector<std::string> busyNTReg;
	std::vector<std::string> busyFTReg;
	//std::vector<std::string> freeFuncReg;
	
	Scope *globalScope;
	Scope *curScope;
	int init_loop;
public:
	std::vector<std::string> regCode;
	std::map<std::string, std::string> gFloatVal;
	//friend class CodeGeneration;
	RegisterAllocation(Scope *globalscope, std::map<std::string, std::string> &floatVal){
		init_loop = 0;
		globalScope = globalscope;
		curScope = globalscope;
		gFloatVal = floatVal;
		for(int i=0;i<10;++i){
			std::string temReg;
			temReg = "$t" + std::to_string(i);			
			freeNTReg.push_back(temReg);
		}
		/*for(int i=0;i<4;++i){
			std::string temReg;
			temReg = "$a" + std::to_string(i);			
			freeNTReg.push_back(temReg);
		}*/
		for(int i=0;i<32;++i){
			std::string temReg;
			temReg = "$f" + std::to_string(i);			
			freeFTReg.push_back(temReg);
		}
		regCode.push_back(".text");
		regCode.push_back("main:");
		curScope->curReg = &((curScope->regTable.find("main"))->second);
		std::string str = "\taddiu $sp,$sp,-" + std::to_string(curScope->curReg->spaceSize);
		regCode.push_back(str);
		AllocateReg(curScope, curScope->mainCode);
		str = "\taddiu $sp,$sp," + std::to_string(curScope->curReg->spaceSize);
		regCode.push_back(str);
		regCode.push_back("\tjr $ra");
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
	int outscopeVarFind(std::string var);

	int AllocateReg(Scope *scope, std::vector<std::string> &ircode);
	void NeedStoreInstsAllocate(std::string ir);

	void MemoryInstAllocate(std::string ir);
	void StoreInstAllocate(std::vector<std::string> &components);
	void LoadInstAllocate(std::vector<std::string> &components);

	void BranchInstsAllocate(std::string ir);

	void ArrayInitAssign(std::vector<std::string> &components);
	void IntArrayInitAssign(std::vector<std::string> &components, int pos);
	void FloatArrayInitAssign(std::vector<std::string> &components, int pos);

	bool LWStackPosFind(RegInfo *rinfo, std::string var, std::string &reg, std::pair<int, Type> &varPair);
	int StackPosFind(RegInfo *rinfo, std::string var);

	void StoreBackOuterVar();
	void StandLibInstAllocate(std::vector<std::string> &components);
};