#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <regex>
#include <string>
#include <iomanip>
#include "scope.cpp"
#include "BackEndHelper.h"

class CodeGeneration{

public:

	std::vector<std::string> assembly;	
	std::vector<std::string> regIRCode;
	std::map<std::string, std::string> gFloatVal;
	CodeGeneration(std::vector<std::string> &RegIRcode, std::map<std::string, std::string> &floatVal){
		regIRCode = RegIRcode;
		gFloatVal = floatVal;
		assembly.push_back(".data");
		std::stringstream ss;
		for(auto it = floatVal.begin(); it != floatVal.end(); ++it){
			ss.str("");
			ss << "\t" << it->second << ": .float " << it->first;
			assembly.push_back(ss.str());
		}
		AssemblyGen();
	}

	void AssemblyGen(){
		for(auto it = regIRCode.begin();it !=regIRCode.end();++it){
			std::string ir = std::regex_replace((*it), std::regex(", "), ",");
			//std::cout << "reg ir code is " << *it << std::endl;
			if(ir.find("assign")==0){
	        	GenAssignInst(ir);
	        }
			else if(isBinaryInstruction(ir)){
				GenArithmeticInst(ir);
			}
			else if(isBranchInstruction(ir)){
				GenBranchInst(ir);
			}
			else if(isFunctionCall(ir)){
				continue;
			}
			else if(ir.find("label")==0){
	        	assembly.push_back(ir);
	        }
	        else if((*it).find("goto")==0){
	        	auto components = split(*it, ',');
	        	std::string label = components[1];
	        	label = "\tj " + label;
	        	assembly.push_back(label);
	        }
	        else{
	        	assembly.push_back(ir);
	        }
		}
	}
	void GenBranchInst(std::string ir){
		auto components = split(ir, ',');
		bool floatArithm = false;
		for(auto s : components){
			if(s[0]=='$' && s[1]=='t'){
				floatArithm = false;
				break;
			}
			else if(s[0]=='$' && s[1]=='f'){
				floatArithm = true;
				break;
			}
		}
		if(floatArithm){
			GenFloatBranchInst(components);
		}
		else{
			GenIntBranchInst(components);
		}		
	}
	void GenFloatBranchInst(std::vector<std::string> components){
		std::stringstream ss;
		if(components[0] == "breq" || components[0] == "brneq"){
	        ss << "\tc.eq.s " << components[1] << "," << components[2];
	    }
	    else if(components[0] == "brgt" || components[0] == "brleq"){
	        ss << "\tc.le.s " << components[1] << "," << components[2];
	    }
	    else if(components[0] == "brlt" || components[0] == "brgeq"){
	        ss << "\tc.lt.s " << components[1] << "," << components[2];
	    }
	    assembly.push_back(ss.str());
	    ss.str("");

	    if(components[0] == "brneq" || components[0] == "brgt" || components[0] == "brgeq"){
	    	ss << "\tbc1f " << components[3];
	    }
	    else ss << "\tbc1t " << components[3];
	    assembly.push_back(ss.str());
	}
	void GenIntBranchInst(std::vector<std::string> components){
		std::stringstream ss;
		if(components[0] == "breq"){
	        ss << "\tbeq ";
	    }
	    else if(components[0] == "brneq"){
	        ss << "\tbne ";
	    }
	    else if(components[0] == "brlt"){
	        ss << "\tblt ";
	    } 
	    else if(components[0] == "brgt"){
	        ss << "\tbgt ";
	    }
	    else if(components[0] == "brgeq"){
	        ss << "\tbge ";
	    } 
	    else if(components[0] == "brleq"){
	        ss << "\tble ";
	    }
	    ss << components[1] << "," << components[2] << "," << components[3];
	    assembly.push_back(ss.str());
	}
	void GenAssignInst(std::string ir){
		auto components = split(ir, ',');
		if(components.size() > 3) return; // 已经处理过了
		bool floatArithm = false;
		for(auto s : components){
			if(s[0]=='$' && s[1]=='t'){
				floatArithm = false;
				break;
			}
			else if(s[0]=='$' && s[1]=='f'){
				floatArithm = true;
				break;
			}
		}
		if(!floatArithm){
			std::string assemblyCode = "\tmove " + components[1] + "," + components[2];
			assembly.push_back(assemblyCode);
		}
		else{
			std::string assemblyCode = "\tmov.s " + components[1] + "," + components[2];
			assembly.push_back(assemblyCode);
		}		
	}
	void GenArithmeticInst(std::string ir){
		auto components = split(ir, ',');
		bool floatArithm = false;
		for(auto s : components){
			if(s[0]=='$' && s[1]=='t'){
				floatArithm = false;
				break;
			}
			else if(s[0]=='$' && s[1]=='f'){
				floatArithm = true;
				break;
			}
		}
		if(!floatArithm){
			GenIntArithmeticInst(components);
		}
		else{
			GenFloatArithmeticInst(components);
		}
	}
	void GenIntArithmeticInst(std::vector<std::string> &components){
		std::stringstream ss;
		ss << "\t" << components[0] << " ";
		if(components[0] == "mult" || components[0] == "div"){
			ss << components[1] << "," << components[2];
			assembly.push_back(ss.str());
			ss.str("");
			ss << "\tmflo " << components[3];
			assembly.push_back(ss.str());
		} 
		else{
			ss << components[3] << "," << components[1] << "," << components[2];
			assembly.push_back(ss.str());
		}
	}
	void GenFloatArithmeticInst(std::vector<std::string> &components){
		std::stringstream ss;
		if(components[0] == "mult"){
			ss << "\t" << "mul.s"  << " ";
		}
		else ss << "\t" << components[0] << ".s"  << " ";
		ss << components[3] << "," << components[1] << "," << components[2];
		assembly.push_back(ss.str());
	}
};