#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <getopt.h>
#include "BackEndHelper.h"
#include "scope.h"
#include "RegisterAllocation.h"
#include "CodeGeneration.h"
#include "LivenessAnalysis.h"
#include "OptRegisterAllocation.h"

#define USAGE               \
"usage:\n"                   \
"  tiger [options]\n"      \
"options:\n"                   \
"  -f [ir_file]  ir file to generate MIPS code\n"    \
"  -o [reg_allocator]  'o' means optimized register allocation, 'u' means unoptimized\n"    \
"  -h              Show this help message\n"


/* OPTIONS DESCRIPTOR ====================================================== */
static struct option gLongOptions[] = {
  {"ir_file",   required_argument,      NULL,           'f'},
  {"reg_allocator",   required_argument,      NULL,     'o'},
  {"help",        no_argument,            NULL,           'h'},
  {NULL,            0,                    NULL,            0}
};

std::vector<std::string> RegisterAllocation::regCode = {".text", "\tj main"};
std::vector<std::string> OptRegisterAllocation::regCode = {".text", "\tj main"};
int LivenessAnalysis::sNum = 0;
int LivenessAnalysis::floatVarNum = 0;
int main(int argc, char *argv[]) {

	char *fileName;
	char *opt; // o means opt, u means unopt
	int option_char = 0;
	while ((option_char = getopt_long(argc, argv, "o:f:h", gLongOptions, NULL)) != -1) {
	    switch (option_char) {
	      default:
	        fprintf(stderr, "%s", USAGE);
	        exit(__LINE__); 
	      case 'o': 
	        opt = optarg;
	        break;                                          
	      case 'f': 
	        fileName = optarg;
	        break;                                          
	      case 'h': // help
	        fprintf(stdout, "%s", USAGE);
	        exit(0);
	        break;
	    }
	}
	std::ifstream IRFile (fileName);
	if (!IRFile.good()) {
	    std::cerr << "File " << fileName << " doesn't exist!\n";
	    exit(0);
	}
	std::string line;
  	std::vector<std::string> buffer;
  	std::map <std::string, FuncInfo> funcInstMap;
  	buffer.clear();
  	int curSize = 0;
  	std::map<std::string, std::pair<int, Type> > varMap;
	while (std::getline(IRFile, line)) { 
	    auto tmp = trim(line);
	    if (tmp.empty()) continue;
	    if (tmp.find("int-list:") == 0 || tmp.find("float-list:") == 0){    	
	        std::string ir = std::regex_replace(tmp, std::regex("\\s+"), "");
	        auto components = split(ir, ':');
	        if(components.size() == 1) continue; // means no var
	        auto vars = split(components[1], ',');
	        for(auto var : vars){
	        	Type t;
	        	if(tmp.find("float-list:") == 0) t.typeVal = "float";
	        	else t.typeVal = "int";
	        	varMap.insert(std::make_pair(var, std::make_pair(curSize, t)));
	        	curSize += 4;
	        }	      
	        continue;
	    }
	    std::string ir = std::regex_replace(tmp, std::regex(", "), ",");
	    buffer.push_back(ir);
	    if(ir.find("#end_function")==0){
	    	auto components = split(ir,' ');
	    	FuncInfo finfo;
	    	finfo.stmts = buffer;
	    	finfo.spaceSize = curSize;
	    	finfo.varMap = varMap;
	    	varMap.clear();
	    	curSize = 0;
	    	funcInstMap.insert(std::make_pair(components[1], finfo));
	    	buffer.clear();
	    }   
	}
	/*for(auto it = funcInstMap.begin(); it != funcInstMap.end(); ++it){
		std::cout << "\n\nfunc " << it->first << std::endl;
		for(auto iter = it->second.varMap.begin(); iter != it->second.varMap.end(); ++iter){
			std::cout << "var " << iter->first << ",pos " << iter->second.first << ", type " << iter->second.second.typeVal << std::endl;
		}
		for(auto iter = it->second.outscopeVarMap.begin(); iter != it->second.outscopeVarMap.end(); ++iter){
			std::cout << "out scope var " << iter->first << ",pos " << iter->second.first << ", type " << iter->second.second.typeVal << std::endl;
		}
	}*/
	funcInstMap["main"].caller = NULL;
	// 在两个func都有同一名称变量就出错，而且没弄对顺序
	for(auto it = funcInstMap.begin(); it != funcInstMap.end(); ++it){
		CheckOutScopeVar(it->second);
		for(auto varit = it->second.outscopeVarMap.begin(); varit != it->second.outscopeVarMap.end(); ++varit){
			std::string var = varit->first;
			for(auto iter = funcInstMap.begin(); iter != funcInstMap.end(); ++iter){
				if(iter->second.varMap.find(var) != iter->second.varMap.end()){
					varit->second.second.typeVal = ((iter->second.varMap.find(var))->second).second.typeVal;
					break;
				}
			}
		}	
	}
	// 把ra给加上
	// 把array的size分配好
	for(auto it = funcInstMap.begin(); it != funcInstMap.end(); ++it){
		Type t;
		t.typeVal = "return address";
		it->second.varMap.insert(std::make_pair("$ra", std::make_pair(it->second.spaceSize,t)));
		it->second.spaceSize += 4;
		for(auto iter = it->second.arrayVar.begin(); iter != it->second.arrayVar.end(); ++iter){
			Type tarray = (it->second.varMap.find(iter->first))->second.second;
			tarray.dimension = iter->second;
			it->second.varMap[iter->first] = std::make_pair(it->second.spaceSize, tarray);
			it->second.spaceSize += 4 * tarray.dimension;
		}
	}
	/*for(auto it = funcInstMap.begin(); it != funcInstMap.end(); ++it){
		std::cout << "\n\nfunc " << it->first << std::endl;
		for(auto iter = it->second.varMap.begin(); iter != it->second.varMap.end(); ++iter){
			std::cout << "var " << iter->first << ",pos " << iter->second.first << ", type " << iter->second.second.typeVal << std::endl;
		}
		for(auto iter = it->second.outscopeVarMap.begin(); iter != it->second.outscopeVarMap.end(); ++iter){
			std::cout << "out scope var " << iter->first << ",pos " << iter->second.first << ", type " << iter->second.second.typeVal << std::endl;
		}
	}*/
	std::map<std::string, std::string> gFloatVal;
	if(*opt == 'u'){
		for(auto it = funcInstMap.begin(); it != funcInstMap.end(); ++it){
			if(it->first != "main"){
				LivenessAnalysis liveAnalyst(it->second, gFloatVal, false);
				std::string str = it->first + ":";
				RegisterAllocation::regCode.push_back(str);
				RegisterAllocation regAllocator(it->second, funcInstMap, gFloatVal);
			}
		}		
		LivenessAnalysis liveAnalyst(funcInstMap["main"], gFloatVal, false);
		std::string str = "\taddiu $sp,$sp,-" + std::to_string(funcInstMap["main"].spaceSize);
		RegisterAllocation::regCode.push_back(str);
		RegisterAllocation::regCode.push_back("main:");
		RegisterAllocation regAllocator(funcInstMap["main"], funcInstMap, gFloatVal);
		str = "\taddiu $sp,$sp," + std::to_string(funcInstMap["main"].spaceSize);
		RegisterAllocation::regCode.push_back(str);
		CodeGeneration MIPSCode(regAllocator.regCode, gFloatVal);
		std::ofstream OpenFile("MIPSCode.s");  
		if(OpenFile.fail()){
			std::cout << "open file failed" << std::endl;  
			exit(0);  
		}
		for(auto it = MIPSCode.assembly.begin(); it != MIPSCode.assembly.end(); ++it){
			//std::cout << (*it) << std::endl;
			OpenFile << (*it) << "\n";
		}
		OpenFile.close();
		std::cout << "Successfully generate MIPS assembly code in file MIPSCode.s using naive register allocation" << std::endl;		
	}
	else if(*opt == 'o'){
		for(auto it = funcInstMap.begin(); it != funcInstMap.end(); ++it){
			if(it->first != "main"){
				LivenessAnalysis liveAnalyst(it->second, gFloatVal);
				std::string str = it->first + ":";
				OptRegisterAllocation::regCode.push_back(str);
				OptRegisterAllocation regAllocator(it->second, funcInstMap, gFloatVal);
			}
		}		
		LivenessAnalysis liveAnalyst(funcInstMap["main"], gFloatVal);
		OptRegisterAllocation::regCode.push_back("main:");
		std::string str = "\taddiu $sp,$sp,-" + std::to_string(funcInstMap["main"].spaceSize);
		OptRegisterAllocation::regCode.push_back(str);
		OptRegisterAllocation regAllocator(funcInstMap["main"], funcInstMap, gFloatVal);
		str = "\taddiu $sp,$sp," + std::to_string(funcInstMap["main"].spaceSize);
		OptRegisterAllocation::regCode.push_back(str);
		CodeGeneration MIPSCode(regAllocator.regCode, gFloatVal);
		std::ofstream OpenFile("MIPSCode.s");  
		if(OpenFile.fail()){
			std::cout << "open file failed" << std::endl;  
			exit(0);  
		}
		for(auto it = MIPSCode.assembly.begin(); it != MIPSCode.assembly.end(); ++it){
			//std::cout << (*it) << std::endl;
			OpenFile << (*it) << "\n";
		}
		OpenFile.close();
		std::cout << "Successfully generate MIPS assembly code in file MIPSCode.s using whole function register allocation" << std::endl;
	}
	else{
		std::cout << "parameter error use './tiger -h' for help" << std::endl;
		exit(1);
	}
	return 0;
}


