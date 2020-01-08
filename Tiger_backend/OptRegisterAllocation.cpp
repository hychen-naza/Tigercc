#include "OptRegisterAllocation.h"
#include "LivenessAnalysis.h"
void OptRegisterAllocation::OptAllocateReg(FuncInfo &rinfo){
	for(int i = 0; i < rinfo.nodes.size(); ++i){
		std::string ir = std::regex_replace(rinfo.nodes[i]->content, std::regex(", "), ",");
		//std::cout << ir << std::endl;
		
		// 必须assign写在isBinaryInstruction前，因为isBinaryInstruction也有assign
		if(ir.find("assign")==0){
        	AssignInstAllocate(rinfo, ir, i);
        }
		else if(isBinaryInstruction(ir)){
			ArithmeticInstAllocate(rinfo, ir, i);
		}		
		else if(isBranchInstruction(ir)){
			BranchInstAllocate(rinfo, ir, i);
		}
		else if(isFunctionDeclStmt(ir)){
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
				StandLibInstAllocate(rinfo, components, i);
				FreeReservedNT();
				continue;
			}	
			int begin = 2;
			if(ir.find("callr")==0){
				begin = 3;
			}	
			std::string funcName = components[begin-1];
			std::stringstream ss;
			// parameter			
			for(int j=begin; j<components.size(); ++j){
				ss.str("");
				std::string reg;
				FindVarRegister(rinfo, components[j], i, reg);
				if(reg.at(1) == 'f'){
					ss << "\tmov.s $f" << j-begin << "," << reg;
				}
				else{
					ss << "\tmove $a" << j-begin << "," << reg;
				}
				regCode.push_back(ss.str());
			}
			ss.str("");
			// release all old var
			// copy 
			std::map<std::string, Webs> copyVarWebMap = rinfo.varWebMap;
			ReleaseAllVarReg(rinfo); //全部存回内存
			FreeReservedNT(); 
			FreeReservedFT();
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
					std::string reg = GetReservedFT();
					ss << "\tl.s " << reg << "," << outScopePos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\ts.s " << reg << "," << pos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");	
					FreeReservedFT();				
				}
				else if((it->second).second.typeVal == "int"){
					ss.str("");
					std::string reg = GetReservedNT();
					ss << "\tlw " << reg << "," << outScopePos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\tsw " << reg << "," << pos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					FreeReservedNT();
				}
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
			// load back
			for(auto it = copyVarWebMap.begin(); it != copyVarWebMap.end(); ++it){				
				if(IsIntNum(it->first) || IsFloatNum(it->first)) continue;
				std::string reg;
				FindVarRegister(rinfo, it->first, i, reg);
			}
			if(begin == 3){												
				std::string tempReg;
				FindVarRegister(rinfo, components[1], i, tempReg);
				if(tempReg[1] == 'f'){
					ss.str("");
					ss << "\tmov.s " << tempReg << ",$f0";
					regCode.push_back(ss.str());
				}
				else{
					ss.str("");
					ss << "\tmove " << tempReg << ",$v1";
					regCode.push_back(ss.str());
				}				
			}		
		}
		else if(ir.find("return")==0){
			auto components = split(ir, ',');
			if(components[1] != ""){
				std::string reg;
				FindVarRegister(rinfo, components[1], i, reg);
				std::stringstream ss;
				if(reg.at(1) == 'f'){
					ss << "\tmov.s $f0," << reg;
				}
				else{
					ss << "\tmove $v1," << reg;
				}
				regCode.push_back(ss.str());
			}
			regCode.push_back("\tjr $ra");
			rinfo.varWebMap.clear(); //means no var in register
		}
		else if(isMemoryInstruction(ir)){
			MemoryInstAllocate(ir, rinfo, i);
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
		FreeReservedNT(); // 就算一个指令3个操作数全是spill web，3个寄存器也够了;...没考虑function call, 4个吧
		FreeReservedFT();
	}
	
}

void OptRegisterAllocation::StandLibInstAllocate(FuncInfo &rinfo, std::vector<std::string> &components, int line){
	regCode.push_back("\tli $v0,1");
	std::string reg;
	std::stringstream ss;
	if(FindVarRegister(rinfo, components[2], line, reg)){
		ss << "\tmove $a0," << reg; 
		regCode.push_back(ss.str());
		regCode.push_back("\tsyscall");
	}
}



void OptRegisterAllocation::BranchInstAllocate(FuncInfo &rinfo, std::string ir, int line){
	auto components = split(ir, ',');
	std::stringstream ss;
	std::string reg;
	std::string defReg;
	for(auto s : components){
		if(FindVarRegister(rinfo, s, line, reg)){
			ss << reg << ",";
		}
		else{
			ss << s << ",";
		}
	}
	regCode.push_back(ss.str());
}
void OptRegisterAllocation::ArithmeticInstAllocate(FuncInfo &rinfo, std::string ir, int line){
	auto components = split(ir, ',');
	std::stringstream ss;
	std::string reg;
	std::string defVar = components[3];
	std::string defReg;
	for(auto s : components){
		if(FindVarRegister(rinfo, s, line, reg)){
			if(defVar == s) defReg = reg;
			ss << reg << ",";
		}
		else{
			ss << s << ",";
		}
	}
	regCode.push_back(ss.str());
	// 检查要不要把def var给存回内存
	StoreVarToMemory(rinfo, defVar, defReg, line);
}


void OptRegisterAllocation::AssignInstAllocate(FuncInfo &rinfo, std::string ir, int line){
	auto components = split(ir, ',');
	if(components.size() > 3){
		ArrayInitAssign(components, rinfo);
		return;
	}
	else if(components.size() == 3){
		std::stringstream ss;
		std::string reg;
		std::string defVar = components[1];
		std::string defReg;

		for(auto s : components){
			if(FindVarRegister(rinfo, s, line, reg)){
				if(defVar == s) defReg = reg;
				ss << reg << ",";
			}
			else{
				ss << s << ",";
			}
		}
		//std::cout << ss.str() << std::endl;
		regCode.push_back(ss.str());
		StoreVarToMemory(rinfo, defVar, defReg, line);
	}
}

void OptRegisterAllocation::ArrayInitAssign(std::vector<std::string> &components, FuncInfo &rinfo){
	std::string reg;
	std::pair<int, Type> varPair;
	int pos = StackPosFind(rinfo, components[1], varPair);
	if(varPair.second.typeVal == "float"){
		FloatArrayInitAssign(components, pos);
	}
	else if(varPair.second.typeVal == "int"){
		IntArrayInitAssign(components, pos);
	}
}

void OptRegisterAllocation::FloatArrayInitAssign(std::vector<std::string> &components, int pos){
	std::stringstream ss;
	ss.str("");
	ss << "\tli $v0," << components[2];
	std::string initValVar = gFloatVal.find(components[3])->second;
	regCode.push_back(ss.str());
	ss.str("");
	std::string addReg = GetReservedNT();
	std::string initValReg = GetReservedFT();
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
void OptRegisterAllocation::IntArrayInitAssign(std::vector<std::string> &components, int pos){
	std::stringstream ss;
	ss.str("");
	ss << "\tli $v0," << components[2];
	std::string initVal = components[3];
	regCode.push_back(ss.str());
	ss.str("");
	std::string reg = GetReservedNT();
	std::string reg1 = GetReservedNT();
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


void OptRegisterAllocation::MemoryInstAllocate(std::string ir, FuncInfo &rinfo, int line){
	auto components = split(ir, ',');
	if(components[0] == "array_store"){
		StoreInstAllocate(components, rinfo, line);
	}
	else if(components[0] == "array_load"){
		LoadInstAllocate(components, rinfo, line);
	}
}
void OptRegisterAllocation::StoreInstAllocate(std::vector<std::string> &components, FuncInfo &rinfo, int line){
	std::string addReg, subscriptReg, storeValReg;
	std::stringstream ss;
	int pos = StackPosFind(rinfo, components[1]);
	addReg = GetReservedNT();
	ss << "\tla " << addReg << "," << pos << "($sp)";
	regCode.push_back(ss.str());
	ss.str("");

	if(IsIntNum(components[2])){
		subscriptReg = GetReservedNT();
		ss.str(""); 
		ss << "\tli " << subscriptReg << "," << components[2];
		regCode.push_back(ss.str());

	}
	else{
		subscriptReg = GetReservedNT();
		std::string originalSubscriptReg;
		FindVarRegister(rinfo, components[2], line, originalSubscriptReg);
		ss.str(""); 
		ss << "\tmove " << subscriptReg << "," << originalSubscriptReg; // 因为下面会改变originalSubscriptReg的值
		regCode.push_back(ss.str());
	}
	ss.str(""); 
	std::string reg = GetReservedNT();
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
	FindVarRegister(rinfo, components[3], line, storeValReg);
	// float int 区分
	if(storeValReg[1] == 'f') {
		std::string tempFTReg = GetReservedFT();
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
	else {
		std::string tempNTReg = GetReservedNT();
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

void OptRegisterAllocation::LoadInstAllocate(std::vector<std::string> &components, FuncInfo &rinfo, int line){
	std::string addReg, subscriptReg, loadVarReg;
	std::pair<int, Type> varPair;
	std::stringstream ss;
	int pos = StackPosFind(rinfo, components[2]);
	addReg = GetReservedNT();
	ss << "\tla " << addReg << "," << pos << "($sp)";
	regCode.push_back(ss.str());
	ss.str("");

	if(IsIntNum(components[3])){
		subscriptReg = GetReservedNT();
		ss.str(""); 
		ss << "\tli " << subscriptReg << "," << components[3];
		regCode.push_back(ss.str());
	}
	else{
		subscriptReg = GetReservedNT();
		std::string originalSubscriptReg;
		FindVarRegister(rinfo, components[3], line, originalSubscriptReg);
		ss.str(""); 
		ss << "\tmove " << subscriptReg << "," << originalSubscriptReg; // 因为下面会改变originalSubscriptReg的值
		regCode.push_back(ss.str());
	}
	ss.str("");
	std::string reg = GetReservedNT();
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
	FindVarRegister(rinfo, components[1], line, loadVarReg);
	// float int 区分
	if(loadVarReg[1] == 'f') {
		std::string tempFTReg = GetReservedFT();
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
	else {
		std::string tempNTReg = GetReservedNT();
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