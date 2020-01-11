#include "OptRegisterAllocation.h"



/*std::string RegisterAllocation::outscopeVarRegFind(std::string var){
	// 这里没有考虑该var被spill的情况
	std::string reg;
	Scope locals = *curScope;
	Scope *s = &locals;
	while(s != NULL){
		while(s->curReg != NULL){
			if(s->curReg->varRegMap.find(var) != s->curReg->varRegMap.end()){
				reg = ((s->curReg->varRegMap.find(var))->second);
				return reg;
			}
			s->curReg = s->curReg->caller;
		}
		s = s->parent;
		s->curReg = &((s->regTable.find("main"))->second);
	}
	return "Error no find reg for var";
}
*/


std::string OptRegisterAllocation::OptAllocateReg(Scope *scope, RegInfo *rinfo){
	for(auto it = curScope->curReg->intWebMap.begin();it != curScope->curReg->intWebMap.end();++it){
		std::cout << " var " << it->first << ":";
		for(auto w : it->second){
			std::cout << " < web :" << w.s << ", reg : " << w.regName; 
		}
		std::cout << std::endl;
	}
	for(int i = 0; i < rinfo->nodes.size(); ++i){

		std::string ir = std::regex_replace(rinfo->nodes[i]->content, std::regex(", "), ",");
		std::cout << ir << std::endl;
		if(ir.find("#enterscope")==0){
			ReleaseDeadVarReg(i);
			auto components = split(ir,'_');
			curScope = globalScope;
			for(int i=components.size()-1; i>=1; i--){
				int pos = std::stoi(components[i]);
				curScope = curScope->scopeKids[pos];
			}
			curScope->curReg = &((curScope->regTable.find("main"))->second);
			RegGraphColor(i);
			std::string str = "\taddiu $sp,$sp,-" + std::to_string(curScope->curReg->spaceSize);
			regCode.push_back(str);
			std::stringstream ss;
			for(auto it = curScope->curReg->outscopeVar.begin(); it != curScope->curReg->outscopeVar.end(); ++it){				
				Type t = (it->second).second;				
				int pos = (it->second).first;
				int outScopePos = RecurFindVarStackPos(it->first);
				// 此时不能用这个var的reg，可能现在还被其他var使用着
				// 使用保留的reg $s0, $f31
				if(t.typeVal == "int"){
					ss.str("");
					ss << "\tlw $s0," << outScopePos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\tsw $s0," << pos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
				}
				else if(t.typeVal == "float"){
					ss.str("");
					ss << "\tl.s $f31," << outScopePos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\ts.s $f31," << pos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
				}
			}
			OptAllocateReg(curScope, curScope->curReg);
		}
		else if(ir.find("#exitscope")==0){
			std::string str = "\taddiu $sp,$sp," + std::to_string(curScope->curReg->spaceSize);
			regCode.push_back(str);
			curScope = curScope->parent;
		}
		// 必须assign写在isBinaryInstruction前，因为isBinaryInstruction也有assign
		if(ir.find("assign")==0){
        	AssignInstAllocate(ir, i);
        }
		else if(isBinaryInstruction(ir) || isBranchInstruction(ir)){
			ArithmeticInstAllocate(ir, i);
		}		
		/*else if(isMemoryInstruction(ir)){
			MemoryInstAllocate(ir, i);
		}*/
		// function decl, load parameters
		else if(ir.find("float")==0 || ir.find("int")==0 || ir.find("void")==0){

	        /*std::string str = std::regex_replace(ir, std::regex("int|void|float| "), "");
	        std::cout << str << std::endl;
	        int pos1 = str.find('(') + 1;
	        int pos2 = str.find(')');
	        std::string param = str.substr(pos1, pos2-pos1);
	        std::cout << param << std::endl;
	        auto paramVec = split(param, ',');
	        //std::cout << "paramVec is " << paramVec << std::endl;
	        for(auto s:paramVec){
	        	std::cout << "param " << s << std::endl;
	        	int pos = RecurFindVarStackPos(s);
	        	std::string reg;
	        	FindVarRegister(s, i, reg);
	        	std::cout << "param " << s << ", reg is " << reg << ", pos is " << pos << std::endl;
	        	std::stringstream ss;
	            if(reg.at(1) == 'f'){
					ss << "\tl.s " << reg << "," << pos << "($sp)";
				}
				else{
					ss << "\tlw " << reg << "," << pos << "($sp)";
				}
				regCode.push_back(ss.str());
	        }*/
		}
		else if(ir.find("call")==0){
			auto components = split(ir,',');
			if(components[1] == "printi"){
				StandLibInstAllocate(components, i);
				continue;
			}			
			int begin = 2;
			if(ir.find("callr")==0){
				begin = 3;
			}			
			std::string funcName = components[begin-1];
			RegInfo *rinfo = &((curScope->regTable.find(funcName))->second);
			rinfo->caller = curScope->curReg;
			std::stringstream ss;
			// store parameter value into stack position		
			for(int j=begin; j<components.size(); ++j){
				ss.str("");
				std::string reg;
				FindVarRegister(components[j], i, reg);
				int pos = 4*(j-begin) - rinfo->spaceSize;
				if(reg.at(1) == 'f'){
					ss << "\ts.s " << reg << "," << pos << "($sp)";
				}
				else{
					ss << "\tsw " << reg << "," << pos << "($sp)";
				}
				regCode.push_back(ss.str());
			}
			ss.str("");
			// release old var in previous reg
			ReleaseDeadVarReg(i);			
			curScope->curReg = rinfo;
			/*for(auto it = curScope->curReg->nodes.begin(); it != curScope->curReg->nodes.end(); ++it){

			}*/
			// color var in this reg
			RegGraphColor(i);
			std::string str = "\taddiu $sp,$sp,-" + std::to_string(curScope->curReg->spaceSize);
			regCode.push_back(str);

			for(auto it = curScope->curReg->outscopeVar.begin(); it != curScope->curReg->outscopeVar.end(); ++it){				
				Type t = (it->second).second;				
				int pos = (it->second).first;
				int outScopePos = RecurFindVarStackPos(it->first);
				// 此时不能用这个var的reg，可能现在还被其他var使用着
				// 使用保留的reg $s0, $f31来存值
				if(t.typeVal == "int"){
					ss.str("");
					ss << "\tlw $s0," << outScopePos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\tsw $s0," << pos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
				}
				else if(t.typeVal == "float"){
					ss.str("");
					ss << "\tl.s $f31," << outScopePos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
					ss << "\ts.s $f31," << pos << "($sp)";
					regCode.push_back(ss.str());
					ss.str("");
				}
			}
			if(begin == 2){
				OptAllocateReg(curScope, curScope->curReg);
				str = "\taddiu $sp,$sp," + std::to_string(curScope->curReg->spaceSize);
				regCode.push_back(str);
				curScope->curReg = curScope->curReg->caller;
			}
			else if(begin == 3){								
				std::string reg = OptAllocateReg(curScope, curScope->curReg);
				str = "\taddiu $sp,$sp," + std::to_string(curScope->curReg->spaceSize);
				regCode.push_back(str);				
				curScope->curReg = curScope->curReg->caller;
				std::string tempReg;
				FindVarRegister(components[1], i, tempReg);
				//int pos = RecurFindVarStackPos(components[1]);
				if(reg != "" && reg[1] != 'f'){
					ss.str("");
					ss << "\tmove " << tempReg << "," << reg;
					regCode.push_back(ss.str());
				}
				else if(reg != "" && reg[1] == 'f'){
					ss.str("");
					ss << "\tmov.s " << tempReg << "," << reg;
					regCode.push_back(ss.str());
				}
			}
							
		}
		else if(ir.find("return")==0){
			auto components = split(ir, ',');
			std::string reg;
			FindVarRegister(components[1], i, reg);
			for(auto it = curScope->curReg->varWebMap.begin(); it != curScope->curReg->varWebMap.end(); ++it){
				FreeWebsReg(it->second);
			}
			// no need to store back to memory, the return is enough
			// ReleaseDeadVarReg(i+1); // make sure all this function var are released
			return reg;
		}
		
		else{
			regCode.push_back(ir);
		}
		FreeReservedNT();
	}
	return "";
}

void OptRegisterAllocation::StandLibInstAllocate(std::vector<std::string> &components, int line){
	regCode.push_back("\tli $v0,1");
	std::string reg;
	std::stringstream ss;
	if(FindVarRegister(components[2], line, reg)){
		ss << "\tmove $a0," << reg; 
		regCode.push_back(ss.str());
		regCode.push_back("\tsyscall");
	}
}

void OptRegisterAllocation::LoadValFromMemorytoReg(std::string var, std::string reg){
	int pos = RecurFindVarStackPos(var);
	std::stringstream ss;
	if(reg.at(1) == 'f'){
		ss << "\tl.s " << reg << "," << pos << "($sp)";
	}
	else{
		ss << "\tlw " << reg << "," << pos << "($sp)";
	}
	regCode.push_back(ss.str());
}


bool OptRegisterAllocation::FindVarRegister(std::string var, int line, std::string &reg){

	bool find = false;
	auto it1 = curScope->curReg->intWebMap.find(var);
	if(it1 != curScope->curReg->intWebMap.end()){
		for(Webs w : it1->second){
			for(Livescope l : w.liveScopes){
				if(line >= l.liveBegin && line <= l.liveEnd){
					reg = w.regName;
					if(isalpha(var.at(0)) && curScope->curReg->varWebMap.find(var) == curScope->curReg->varWebMap.end()){
						LoadValFromMemorytoReg(var, reg);
					}
					curScope->curReg->varWebMap[var] = w;					
					find = true;
				}
			}			
		}
	}
	auto it2 = curScope->curReg->floatWebMap.find(var);
	if(it2 != curScope->curReg->floatWebMap.end()){
		for(Webs w : it2->second){
			for(Livescope l : w.liveScopes){
				if(line >= l.liveBegin && line <= l.liveEnd){
					reg = w.regName;
					if(isalpha(var.at(0)) && curScope->curReg->varWebMap.find(var) == curScope->curReg->varWebMap.end()){
						LoadValFromMemorytoReg(var, reg);
					}
					curScope->curReg->varWebMap[var] = w;					
					find = true;

				}
			}			
		}
	}
	printf("here3\n");
	auto it3 = curScope->curReg->spilledVar.find(var);
	if(it3 != curScope->curReg->spilledVar.end()){
		reg = GetReservedNT();
		std::stringstream ss;
		ss << "\tli " << reg << "," << it3->second << "($sp)";
		regCode.push_back(ss.str());
		find = true;
	}
	printf("here4\n");
	// make this reg as true, means we use it
	if(find) AllocateReg(reg);
	std::stringstream ss;
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
}

// branch inst也是如此
void OptRegisterAllocation::ArithmeticInstAllocate(std::string ir, int line){
	auto components = split(ir, ',');
	std::stringstream ss;
	std::string reg;
	for(auto s : components){
		if(FindVarRegister(s, line, reg)){
			ss << reg << ",";
		}
		else{
			ss << s << ",";
		}
	}
	regCode.push_back(ss.str());
}


void OptRegisterAllocation::AssignInstAllocate(std::string ir, int line){
	auto components = split(ir, ',');
	if(components.size() > 3){
		std::cout << "not implement yet" <<std::endl; 
	}
	else if(components.size() == 3){
		std::stringstream ss;
		std::string reg;
		for(auto s : components){
			if(FindVarRegister(s, line, reg)){
				ss << reg << ",";
			}
			else{
				ss << s << ",";
			}
		}
		regCode.push_back(ss.str());
	}
}

/*void OptRegisterAllocation::MemoryInstAllocate(std::string ir, int line){
	auto components = split(ir, ',');
	if(components[0] == "store"){
		StoreInstAllocate(components, line);
	}
	else if(components[0] == "load"){
		LoadInstAllocate(components, line);
	}
}
void OptRegisterAllocation::StoreInstAllocate(std::vector<std::string> &components, int line){
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
}*/

/*int StackPosFind(RegInfo *rinfo, std::string var){

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
*/