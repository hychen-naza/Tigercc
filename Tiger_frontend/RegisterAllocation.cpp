#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <regex>
#include <string>
#include <iomanip>
#include "RegisterAllocation.h"

int RegisterAllocation::outscopeVarFind(std::string var){
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

void RegisterAllocation::StoreBackOuterVar(){
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
}

int RegisterAllocation::AllocateReg(Scope *scope, std::vector<std::string> &ircode){
	Scope *s = scope;
	// ircode 再把mainCode给加上
	// 我不太确定插入当前scope中的
	for(auto it = ircode.begin();it !=ircode.end();++it){
		std::string ir = std::regex_replace((*it), std::regex(", "), ",");
		if(ir.find("#enterscope")==0){
			auto components = split(ir,'_');
			curScope = globalScope;
			for(int i=components.size()-1; i>=1; i--){
				int pos = std::stoi(components[i]);
				curScope = curScope->scopeKids[pos];
			}
			curScope->curReg = &((curScope->regTable.find("main"))->second);
			std::string str = "\taddiu $sp,$sp,-" + std::to_string(curScope->curReg->spaceSize);
			regCode.push_back(str);
			std::stringstream ss;
			for(auto it = curScope->curReg->outscopeVar.begin(); it != curScope->curReg->outscopeVar.end(); ++it){
				Type t = (it->second).second;
				int pos = (it->second).first;
				int outScopePos = outscopeVarFind(it->first);
				if(t.typeVal == "int"){
					ss.str("");
					std::string reg = NaiveAllocateNTReg();
					ss << "\tlw " << reg << "," << outScopePos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\tsw " << reg << "," << pos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
				}
				else if(t.typeVal == "float"){
					ss.str("");
					std::string reg = NaiveAllocateFTReg();
					ss << "\tl.s " << reg << "," << outScopePos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\ts.s " << reg << "," << pos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
				}
			}
			// 在进入AllocateReg之前把直接用过的释放掉
			NaiveFreeAllReg();
			AllocateReg(curScope, curScope->mainCode);
		}
		else if(ir.find("#exitscope")==0){			
			StoreBackOuterVar();
			std::string str = "\taddiu $sp,$sp," + std::to_string(curScope->curReg->spaceSize);
			regCode.push_back(str);
			curScope = curScope->parent;
		}

		else if(ir.find("call")==0){
			auto components = split(ir,',');
			if(components[1] == "printi"){
				StandLibInstAllocate(components);
				continue;
			}			
			int begin = 2;
			if(ir.find("callr")==0){
				begin = 3;
			}			
			std::string funcName = components[begin-1];
			RegInfo *rinfo = &((curScope->regTable.find(funcName))->second);
			rinfo->caller = curScope->curReg;
			std::string reg;
			std::pair<int, Type> varPair;
			std::stringstream ss;
			// parameter			
			for(int i=begin; i<components.size(); ++i){
				ss.str("");
				LWStackPosFind(curScope->curReg, components[i], reg, varPair);
				int pos = 4*(i-begin) - rinfo->spaceSize;
				if(varPair.second.typeVal == "float"){
					ss << "\ts.s " << reg << "," << pos << "($sp)";
				}
				else if(varPair.second.typeVal == "int"){
					ss << "\tsw " << reg << "," << pos << "($sp)";
				}
				regCode.push_back(ss.str());
			}
			ss.str("");
			NaiveFreeAllReg();
			// other vars defined in this scope, but not used as parameter
			curScope->curReg = rinfo;
			std::string str = "\taddiu $sp,$sp,-" + std::to_string(curScope->curReg->spaceSize);
			regCode.push_back(str);

			for(auto it = curScope->curReg->outscopeVar.begin(); it != curScope->curReg->outscopeVar.end(); ++it){
				Type t = (it->second).second;				
				int pos = (it->second).first;
				int outScopePos = outscopeVarFind(it->first);
				if(t.typeVal == "int"){
					ss.str("");
					std::string reg = NaiveAllocateNTReg();
					ss << "\tlw " << reg << "," << outScopePos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\tsw " << reg << "," << pos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
				}
				else if(t.typeVal == "float"){
					ss.str("");
					std::string reg = NaiveAllocateFTReg();
					ss << "\tl.s " << reg << "," << outScopePos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\ts.s " << reg << "," << pos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
				}
				NaiveFreeAllReg();
			}
			std::vector<std::string> funcInsts;
			std::string instBegin = "#start_function " + funcName;
			auto itBegin = find(curScope->funcCode.begin(),curScope->funcCode.end(), instBegin);  	
    		itBegin ++;
    		std::string instEnd = "#end_function " + funcName;
    		auto itEnd = find(curScope->funcCode.begin(),curScope->funcCode.end(), instEnd);   
			funcInsts.assign(itBegin, itEnd);
			// 在进入AllocateReg之前把直接用过的释放掉
			
			if(begin == 3){								
				int funcRet = AllocateReg(curScope, funcInsts);
				StoreBackOuterVar();
				str = "\taddiu $sp,$sp," + std::to_string(curScope->curReg->spaceSize);
				regCode.push_back(str);
				curScope->curReg = curScope->curReg->caller;
				std::pair<int, Type> varPair;
				std::string reg;
				LWStackPosFind(curScope->curReg, components[1], reg, varPair);
				if(funcRet != -1 && reg[1] != 'f'){
					std::string reg1 = NaiveAllocateNTReg(); 
					ss.str("");
					ss << "\tlw " << reg1 << "," << (funcRet - rinfo->spaceSize) << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\tmove " << reg << "," << reg1;
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\tsw " << reg << "," << varPair.first << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
				}
				else if(funcRet != -1 && reg[1] == 'f'){
					std::string reg1 = NaiveAllocateFTReg(); 
					ss.str("");
					ss << "\tl.s " << reg1 << "," << (funcRet - rinfo->spaceSize) << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\tmov.s " << reg << "," << reg1;
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\ts.s " << reg << "," << varPair.first << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
				}
			}
			else if(begin == 2){
				AllocateReg(curScope,funcInsts);
				StoreBackOuterVar();
				str = "\taddiu $sp,$sp," + std::to_string(curScope->curReg->spaceSize);
				regCode.push_back(str);
				curScope->curReg = curScope->curReg->caller;
			}		
		}
		else if(isBinaryInstruction(ir)){
			NeedStoreInstsAllocate(ir);
		}
		else if(isMemoryInstruction(ir)){
			MemoryInstAllocate(ir);
		}
		else if(isBranchInstruction(ir)){
			BranchInstsAllocate(ir);
		}
		else if(ir.find("return")==0){
			auto components = split(ir, ',');
			return StackPosFind(curScope->curReg, components[1]);
		}
		else{
			regCode.push_back(ir);
		}
		NaiveFreeAllReg();
	}
	return -1;
}
void RegisterAllocation::StandLibInstAllocate(std::vector<std::string> &components){
	regCode.push_back("\tli $v0,1");
	std::string reg;
	std::pair<int, Type> varPair;
	LWStackPosFind(curScope->curReg, components[2], reg, varPair);
	std::stringstream ss;
	ss << "\tmove $a0," << reg; //因为我的free reg中就没有a0，所以a0一直是空闲的。。。
	regCode.push_back(ss.str());
	regCode.push_back("\tsyscall");
}
void RegisterAllocation::StoreInstAllocate(std::vector<std::string> &components){
	std::string addReg, subscriptReg, storeValReg;
	std::pair<int, Type> varPair;
	std::stringstream ss;
	int pos = StackPosFind(curScope->curReg, components[1]);
	addReg = NaiveAllocateNTReg();
	ss << "\tla " << addReg << "," << pos << "($sp)";
	regCode.push_back(ss.str());
	ss.str("");
	LWStackPosFind(curScope->curReg, components[2], subscriptReg, varPair);
	std::string reg = NaiveAllocateNTReg();
	ss << "\tli " << reg << ",4";
	regCode.push_back(ss.str());
	ss.str(""); 
	ss << "\tmult " << subscriptReg << "," << reg;
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\tmflo " << subscriptReg;
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\tadd " << addReg << "," << addReg << "," << subscriptReg;
	regCode.push_back(ss.str());
	ss.str("");
	LWStackPosFind(curScope->curReg, components[3], storeValReg, varPair);
	// float int 区分
	if(varPair.second.typeVal == "float"){
		std::string tempFTReg = NaiveAllocateFTReg();
		ss << "\tl.s " << tempFTReg << ",(" << addReg << ")";
		regCode.push_back(ss.str());
		ss.str("");
		ss << "\tmov.s " << tempFTReg << "," << storeValReg;
		regCode.push_back(ss.str());
		ss.str("");
		ss << "\ts.s " << tempFTReg << ",(" << addReg << ")";
		regCode.push_back(ss.str());
		ss.str("");
	}
	else if(varPair.second.typeVal == "int"){
		std::string tempNTReg = NaiveAllocateNTReg();
		ss << "\tlw " << tempNTReg << ",(" << addReg << ")";
		regCode.push_back(ss.str());
		ss.str("");
		ss << "\tmove " << tempNTReg << "," << storeValReg;
		regCode.push_back(ss.str());
		ss.str("");
		ss << "\tsw " << tempNTReg << ",(" << addReg << ")";
		regCode.push_back(ss.str());
		ss.str("");
	}
}
void RegisterAllocation::LoadInstAllocate(std::vector<std::string> &components){
	std::string addReg, subscriptReg, loadVarReg;
	std::pair<int, Type> varPair;
	std::stringstream ss;
	int pos = StackPosFind(curScope->curReg, components[2]);
	addReg = NaiveAllocateNTReg();
	ss << "\tla " << addReg << "," << pos << "($sp)";
	regCode.push_back(ss.str());
	ss.str("");
	LWStackPosFind(curScope->curReg, components[3], subscriptReg, varPair);
	std::string reg = NaiveAllocateNTReg();
	ss << "\tli " << reg << ",4";
	regCode.push_back(ss.str());
	ss.str(""); 
	ss << "\tmult " << subscriptReg << "," << reg;
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\tmflo " << subscriptReg;
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\tadd " << addReg << "," << addReg << "," << subscriptReg;
	regCode.push_back(ss.str());
	ss.str("");
	LWStackPosFind(curScope->curReg, components[1], loadVarReg, varPair);
	// float int 区分
	if(varPair.second.typeVal == "float"){
		std::string tempFTReg = NaiveAllocateFTReg();
		ss << "\tl.s " << tempFTReg << ",(" << addReg << ")";
		regCode.push_back(ss.str());
		ss.str("");
		ss << "\tmov.s " << loadVarReg << "," << tempFTReg;
		regCode.push_back(ss.str());
		ss.str("");
		ss << "\ts.s " << loadVarReg << "," << varPair.first << "($sp)";
		regCode.push_back(ss.str());
		ss.str("");
	}
	else if(varPair.second.typeVal == "int"){
		std::string tempNTReg = NaiveAllocateNTReg();
		ss << "\tlw " << tempNTReg << ",(" << addReg << ")";
		regCode.push_back(ss.str());
		ss.str("");
		ss << "\tmove " << loadVarReg << "," << tempNTReg;
		regCode.push_back(ss.str());
		ss.str("");
		ss << "\tsw " << loadVarReg << "," << varPair.first << "($sp)";
		regCode.push_back(ss.str());
		ss.str("");
	}
}
void RegisterAllocation::MemoryInstAllocate(std::string ir){
	auto components = split(ir, ',');
	if(components[0] == "store"){
		StoreInstAllocate(components);
	}
	else if(components[0] == "load"){
		LoadInstAllocate(components);
	}
}

int RegisterAllocation::StackPosFind(RegInfo *rinfo, std::string var){
	auto it1 = rinfo->defVar.find(var);
	if(it1 != rinfo->defVar.end()){
		return it1->second.first;
	}
	auto it2 = rinfo->tempVar.find(var);
	if(it2 != rinfo->tempVar.end()){
		return it2->second.first;
	}
	auto it3 = rinfo->outscopeVar.find(var);
	if(it3 != rinfo->outscopeVar.end()){
		return it3->second.first;
	}
	return -1;
}
bool RegisterAllocation::LWStackPosFind(RegInfo *rinfo, std::string var, std::string &reg, std::pair<int, Type> &varPair){
	std::stringstream ss;
	auto it1 = rinfo->defVar.find(var);	
	if(it1 != rinfo->defVar.end()){
		varPair = it1->second;
		int stackPos = it1->second.first;
		if(it1->second.second.typeVal == "int"){
			reg = NaiveAllocateNTReg();
			ss << "\tlw " << reg << "," << stackPos << "($sp)";
			regCode.push_back(ss.str());
		}
		else if(it1->second.second.typeVal == "float"){
			reg = NaiveAllocateFTReg();
			ss << "\tl.s " << reg << "," << stackPos << "($sp)"; 
			regCode.push_back(ss.str());
		}
		return true;
	}
	auto it2 = rinfo->tempVar.find(var);
	if(it2 != rinfo->tempVar.end()){
		varPair = it2->second;
		int stackPos = it2->second.first;
		if(it2->second.second.typeVal == "int"){
			reg = NaiveAllocateNTReg();
			ss << "\tlw " << reg << "," << stackPos << "($sp)";
			regCode.push_back(ss.str());
		}
		else if(it2->second.second.typeVal == "float"){
			reg = NaiveAllocateFTReg();
			ss << "\tl.s " << reg << "," << stackPos << "($sp)"; 
			regCode.push_back(ss.str());
		}
		return true;
	}
	auto it3 = rinfo->outscopeVar.find(var);
	if(it3 != rinfo->outscopeVar.end()){
		varPair = it3->second;
		int stackPos = it3->second.first;
		if(it3->second.second.typeVal == "int"){
			reg = NaiveAllocateNTReg();
			ss << "\tlw " << reg << "," << stackPos << "($sp)";
			regCode.push_back(ss.str());
		}
		else if(it3->second.second.typeVal == "float"){
			reg = NaiveAllocateFTReg();
			ss << "\tl.s " << reg << "," << stackPos << "($sp)"; 
			regCode.push_back(ss.str());
		}
		return true;
	}
	if(IsIntNum(var)){
		reg = NaiveAllocateNTReg();
		ss << "\tli " << reg << "," << var;
		regCode.push_back(ss.str());
		return true;
	}
	if(IsFloatNum(var)){
		reg = NaiveAllocateFTReg();
		std::string floatVar = (gFloatVal.find(var))->second;
		ss << "\tl.s " << reg << "," << floatVar;
		regCode.push_back(ss.str());
		return true;
	}
	return false;
}

void RegisterAllocation::BranchInstsAllocate(std::string ir){
	auto components = split(ir, ',');
	std::stringstream ss;
	std::string reg;
	std::pair<int, Type> varPair;
	for(auto s : components){	
		if(LWStackPosFind(curScope->curReg, s, reg, varPair)){
			ss << reg << ",";
		}
		else{
			ss << s << ",";
		}	
	}	
	regCode.push_back(ss.str());
}

void RegisterAllocation::ArrayInitAssign(std::vector<std::string> &components){
	std::string reg;
	std::pair<int, Type> varPair;
	LWStackPosFind(curScope->curReg, components[1], reg, varPair);
	if(varPair.second.typeVal == "float"){
		FloatArrayInitAssign(components, varPair.first);
	}
	else if(varPair.second.typeVal == "int"){
		IntArrayInitAssign(components, varPair.first);
	}
}
void RegisterAllocation::FloatArrayInitAssign(std::vector<std::string> &components, int pos){
	std::stringstream ss;
	ss.str("");
	ss << "\tli $v0," << components[2];
	std::string initValVar = gFloatVal.find(components[3])->second;
	regCode.push_back(ss.str());
	ss.str("");
	std::string addReg = NaiveAllocateNTReg();
	std::string initValReg = NaiveAllocateFTReg();
	ss << "\tla " << addReg << "," << pos << "($sp)";
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\tl.s " << initValReg << "," << initValVar;
	regCode.push_back(ss.str());
	ss.str("");
	ss << "loop" << init_loop << ":";
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\ts.s "<< initValReg << ",(" << addReg << ")";
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\taddi " << addReg << "," << addReg << ",4";
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\taddi $v0,$v0,-1";
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\tbnez $v0,loop" << init_loop;
	regCode.push_back(ss.str());
	ss.str("");
	init_loop++;	
}	
void RegisterAllocation::IntArrayInitAssign(std::vector<std::string> &components, int pos){
	std::stringstream ss;
	ss.str("");
	ss << "\tli $v0," << components[2];
	std::string initVal = components[3];
	regCode.push_back(ss.str());
	ss.str("");
	std::string reg = NaiveAllocateNTReg();
	std::string reg1 = NaiveAllocateNTReg();
	ss << "\tla " << reg << "," << pos << "($sp)";
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\tli " << reg1 << "," << initVal;
	regCode.push_back(ss.str());
	ss.str("");
	ss << "loop" << init_loop << ":";
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\tsw "<< reg1 << ",(" << reg << ")";
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\taddi " << reg << "," << reg << ",4";
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\taddi $v0,$v0,-1";
	regCode.push_back(ss.str());
	ss.str("");
	ss << "\tbnez $v0,loop" << init_loop;
	regCode.push_back(ss.str());
	ss.str("");
	init_loop++;	
}	
void RegisterAllocation::NeedStoreInstsAllocate(std::string ir){

	auto components = split(ir, ',');
	if(components[0] == "assign" && components.size() == 4){
		ArrayInitAssign(components);
		return;
	}
	std::stringstream ss;
	std::string reg, firstReg;
	std::pair<int, Type> varPair;
	std::pair<int, Type> firstVarPair;
	int i = 0;
	for(auto s : components){	
		if(LWStackPosFind(curScope->curReg, s, reg, varPair)){
			ss << reg << ",";
			if(i==1){
				firstReg = reg;
				firstVarPair = varPair;
			}
		}
		else{
			ss << s << ",";
		}	
		i++;
	}
	regCode.push_back(ss.str());
	ss.str("");
	// assign 是存第一个var,其余binary inst是存最后一个var
	if(components[0] == "assign"){
		reg = firstReg;
		varPair = firstVarPair;
	}
	if(varPair.second.typeVal == "float"){
		ss << "\ts.s " << reg << "," << varPair.first << "($sp)";
	}
	else if(varPair.second.typeVal == "int"){
		ss << "\tsw " << reg << "," << varPair.first << "($sp)";
	}
	regCode.push_back(ss.str());
}