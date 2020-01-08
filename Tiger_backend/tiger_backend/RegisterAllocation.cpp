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



void RegisterAllocation::AllocateReg(FuncInfo &rinfo){

	for(auto it = rinfo.stmts.begin();it !=rinfo.stmts.end();++it){
		std::string ir = std::regex_replace((*it), std::regex(", "), ",");
		//std::cout << ir << std::endl;
		if(isFunctionDeclStmt(ir)){
			std::string str = std::regex_replace(ir, std::regex("int|void|float| "), "");
	        int pos1 = str.find('(') + 1;
	        int pos2 = str.find(')');
	        std::string param = str.substr(pos1, pos2-pos1);
	        auto paramVec = split(param, ',');
	        std::stringstream ss;
	        for(int i=0; i<paramVec.size(); ++i){
	        	std::pair<int, Type> varPair;
	        	int pos = StackPosFind(rinfo, paramVec[i], varPair);
	        	if(varPair.second.typeVal == "float"){
					ss << "\ts.s $f" << i << "," << pos << "($sp)";
				}
				else if(varPair.second.typeVal == "int"){
					ss << "\tsw $a" << i << "," << pos << "($sp)";
				}
				regCode.push_back(ss.str());
				ss.str("");
	        }	        
		}
		else if(ir.find("call")==0){
			auto components = split(ir,',');
			if(components[1] == "printi"){
				StandLibInstAllocate(components, rinfo);
				NaiveFreeAllReg();
				continue;
			}			
			int begin = 2;
			if(ir.find("callr")==0){
				begin = 3;
			}
			std::string funcName = components[begin-1];			
			std::string reg;
			std::pair<int, Type> varPair;
			std::stringstream ss;
			// parameter			
			for(int i=begin; i<components.size(); ++i){
				ss.str("");
				LWStackPosFind(rinfo, components[i], reg, varPair);
				if(varPair.second.typeVal == "float"){
					ss << "\tmov.s $f" << i-begin << "," << reg;
				}
				else if(varPair.second.typeVal == "int"){
					ss << "\tmove $a" << i-begin << "," << reg;
				}
				regCode.push_back(ss.str());
			}
			ss.str("");
			NaiveFreeAllReg();
			// 在进入AllocateReg之前把用过的释放掉
			FuncInfo *calleeRinfo = &funcMap[funcName];
			calleeRinfo->caller = &rinfo;
			std::string str = "\taddiu $sp,$sp,-" + std::to_string(calleeRinfo->spaceSize);
			regCode.push_back(str);
			for(auto it = calleeRinfo->outscopeVarMap.begin(); it != calleeRinfo->outscopeVarMap.end(); ++it){
				std::string var = it->first;			
				int pos = (it->second).first;				
				int outScopePos = OutscopeVarFind(var, calleeRinfo->caller, calleeRinfo->spaceSize);								
				if((it->second).second.typeVal == "float"){
					ss.str("");
					std::string reg = NaiveAllocateFTReg();
					ss << "\tl.s " << reg << "," << outScopePos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\ts.s " << reg << "," << pos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
				}
				else if((it->second).second.typeVal == "int"){
					ss.str("");
					std::string reg = NaiveAllocateNTReg();
					ss << "\tlw " << reg << "," << outScopePos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\tsw " << reg << "," << pos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
				}
				NaiveFreeAllReg();
			}
			ss.str("");
			int raPos = StackPosFind(*calleeRinfo, "$ra");
			ss << "\tsw $ra," << raPos << "($sp)";
			regCode.push_back(ss.str());
			ss.str("");
			str = "\tjal " + funcName;
			regCode.push_back(str);
			ss << "\tlw $ra," << raPos << "($sp)";
			regCode.push_back(ss.str());
			ss.str("");
			
			StoreBackOuterVar(calleeRinfo);
			str = "\taddiu $sp,$sp," + std::to_string(calleeRinfo->spaceSize);
			regCode.push_back(str);
			if(begin == 3){		
				// return
				std::pair<int, Type> varPair;
				int pos = StackPosFind(rinfo, components[1], varPair);
				ss.str("");			
				if(varPair.second.typeVal == "float"){
					ss << "\ts.s $f0," << pos << "($sp)";
				}
				else if(varPair.second.typeVal == "int"){
					ss << "\tsw $v1," << pos << "($sp)";
				}
				regCode.push_back(ss.str());
			}				
		}
		else if(ir.find("return")==0){
			auto components = split(ir, ',');
			if(components[1] != ""){
				std::pair<int, Type> varPair;
				std::string reg;
				LWStackPosFind(rinfo, components[1], reg, varPair);
				std::stringstream ss;
				if(varPair.second.typeVal == "float"){
					ss << "\tmov.s $f0," << reg;
				}
				else if(varPair.second.typeVal == "int"){
					ss << "\tmove $v1," << reg;
				}
				regCode.push_back(ss.str());
			}
			regCode.push_back("\tjr $ra");
		}
		else if(isBinaryInstruction(ir)){
			NeedStoreInstsAllocate(ir, rinfo);
		}
		else if(isMemoryInstruction(ir)){
			MemoryInstAllocate(ir, rinfo);
		}
		else if(isBranchInstruction(ir)){
			BranchInstsAllocate(ir, rinfo);
		}
		else if(ir.find("#start_function")==0 || ir.find("#end_function")==0){
			continue;
		}
		else if(ir.back()==':'){
			std::string funcName = ir.substr(0, ir.length()-1);
			if(funcMap.find(funcName) != funcMap.end()) continue;
			else regCode.push_back(ir);
		}
		else{
			regCode.push_back(ir);
		}
		NaiveFreeAllReg();
	}
}
bool RegisterAllocation::LWStackPosFind(FuncInfo &rinfo, std::string var, std::string &reg, std::pair<int, Type> &varPair){
	std::stringstream ss;
	auto it1 = rinfo.varMap.find(var);	
	if(it1 != rinfo.varMap.end()){
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
	auto it2 = rinfo.outscopeVarMap.find(var);	
	if(it2 != rinfo.outscopeVarMap.end()){
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
void RegisterAllocation::StandLibInstAllocate(std::vector<std::string> &components, FuncInfo &rinfo){
	std::string reg;
	std::pair<int, Type> varPair;
	LWStackPosFind(rinfo, components[2], reg, varPair);
	std::stringstream ss;
	if(reg.find("$f") == 0) {
		regCode.push_back("\tli $v0,2");
		ss << "\tmov.s $f12," << reg; //打印浮点数
	} else {
		regCode.push_back("\tli $v0,1");
		ss << "\tmove $a0," << reg; //因为我的free reg中就没有a0，所以a0一直是空闲的。。。
	}
	regCode.push_back(ss.str());
	regCode.push_back("\tsyscall");
}
void RegisterAllocation::StoreInstAllocate(std::vector<std::string> &components, FuncInfo &rinfo){
	std::string addReg, subscriptReg, storeValReg;
	std::pair<int, Type> varPair;
	std::stringstream ss;
	int pos = StackPosFind(rinfo, components[1]);
	addReg = NaiveAllocateNTReg();
	ss << "\tla " << addReg << "," << pos << "($sp)";
	regCode.push_back(ss.str());
	ss.str("");
	LWStackPosFind(rinfo, components[2], subscriptReg, varPair);
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
	LWStackPosFind(rinfo, components[3], storeValReg, varPair);
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


void RegisterAllocation::LoadInstAllocate(std::vector<std::string> &components, FuncInfo &rinfo){
	std::string addReg, subscriptReg, loadVarReg;
	std::pair<int, Type> varPair;
	std::stringstream ss;
	int pos = StackPosFind(rinfo, components[2]);
	addReg = NaiveAllocateNTReg();
	ss << "\tla " << addReg << "," << pos << "($sp)";
	regCode.push_back(ss.str());
	ss.str("");
	LWStackPosFind(rinfo, components[3], subscriptReg, varPair);
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
	LWStackPosFind(rinfo, components[1], loadVarReg, varPair);
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
void RegisterAllocation::MemoryInstAllocate(std::string ir, FuncInfo &rinfo){
	auto components = split(ir, ',');
	if(components[0] == "array_store"){
		StoreInstAllocate(components, rinfo);
	}
	else if(components[0] == "array_load"){
		LoadInstAllocate(components, rinfo);
	}
}

int RegisterAllocation::StackPosFind(FuncInfo &rinfo, std::string var){
	auto it1 = rinfo.varMap.find(var);
	if(it1 != rinfo.varMap.end()){
		return it1->second.first;
	}
	auto it2 = rinfo.outscopeVarMap.find(var);
	if(it2 != rinfo.outscopeVarMap.end()){
		return it2->second.first;
	}
	return -1;
}


void RegisterAllocation::BranchInstsAllocate(std::string ir, FuncInfo &rinfo){
	auto components = split(ir, ',');
	std::stringstream ss;
	std::string reg;
	std::pair<int, Type> varPair;
	for(auto s : components){	
		if(LWStackPosFind(rinfo, s, reg, varPair)){
			ss << reg << ",";
		}
		else{
			ss << s << ",";
		}	
	}	
	regCode.push_back(ss.str());
}

void RegisterAllocation::ArrayInitAssign(std::vector<std::string> &components, FuncInfo &rinfo){
	std::string reg;
	std::pair<int, Type> varPair;
	LWStackPosFind(rinfo, components[1], reg, varPair);
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
void RegisterAllocation::NeedStoreInstsAllocate(std::string ir, FuncInfo &rinfo){

	auto components = split(ir, ',');
	if(components[0] == "assign" && components.size() == 4){
		ArrayInitAssign(components, rinfo);
		return;
	}
	std::stringstream ss;
	std::string reg, firstReg;
	std::pair<int, Type> varPair;
	std::pair<int, Type> firstVarPair;
	int i = 0;
	for(auto s : components){	
		if(LWStackPosFind(rinfo, s, reg, varPair)){
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