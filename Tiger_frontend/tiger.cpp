#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include "antlr4-runtime.h"
#include "tigerLexer.h"
#include "tigerParser.h"
#include "SymbolTable.cpp"
#include "SemanticCheck.cpp"
#include "IRGeneration.cpp"
#include "LivenessAnalysis.cpp"
#include "RegisterAllocation.h"
#include "OptRegisterAllocation.h"
#include "CodeGeneration.h"
using namespace antlr4;
using namespace std;

//extern std::vector<IRNode *> LivenessAnalysis(std::vector<std::string> stmts);

bool lexical_parser_error = false;

class MyErrorListener : public BaseErrorListener{
	void syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line,
                             size_t charPositionInLine, const std::string &msg, std::exception_ptr e){
		lexical_parser_error = true;
		std::cout << "Error : line " << line << ":" << charPositionInLine << ", " << msg << std::endl;
	}
};

void RearrangeIR(Scope *scope, std::vector<std::string> &IRcode){
	Scope *s = scope;
	for(auto it = s->regTable.begin(); it != s->regTable.end(); ++it){
		it->second.spaceSize = it->second.curSize;
		/*std::cout << "func:" << it->first << std::endl;
		for(auto iter = it->second.defVar.begin(); iter != it->second.defVar.end(); iter++){
			std::cout << "<" << iter->first << "," << (iter->second).first << "," << (iter->second).second.typeVal<< ">" << std::endl;
		}
		for(auto iter = it->second.tempVar.begin(); iter != it->second.tempVar.end(); iter++){
			std::cout << "<" << iter->first << "," << (iter->second).first << "," << (iter->second).second.typeVal<< ">" << std::endl;
		}
		for(auto iter = it->second.outscopeVar.begin(); iter != it->second.outscopeVar.end(); iter++){
			std::cout << "<" << iter->first << "," << (iter->second).first << "," << (iter->second).second.typeVal<< ">" << std::endl;
		}*/
	}	
	for(auto it = s->funcCode.begin(); it != s->funcCode.end(); ++it){
		std::cout << (*it) << std::endl;
		IRcode.push_back((*it));
	}
	for(auto it = s->mainCode.begin(); it != s->mainCode.end(); ++it){
		std::cout << (*it) << std::endl;
		IRcode.push_back((*it));
	}
	for(auto it = s->scopeKids.begin(); it != s->scopeKids.end(); ++it){
		RearrangeIR((*it), IRcode);
	}
}
int main(int argc, char *argv[]) {
	string line;
	char *fileName = argv[1];
	ifstream tigerFile (fileName);
	if (tigerFile.is_open()) {
		ANTLRInputStream input(tigerFile);

		MyErrorListener *myErrorListener = new MyErrorListener();
		tigerLexer lexer(&input);
		lexer.removeErrorListeners();
    	lexer.addErrorListener(myErrorListener);
		
		CommonTokenStream tokens(&lexer);
		std::vector<std::string> ruleNames = lexer.getRuleNames();
		tokens.fill();
		tigerParser parser(&tokens);
		parser.removeParseListeners();
    	parser.removeErrorListeners();
    	parser.addErrorListener(myErrorListener);
		tree::ParseTree *tree = parser.program();

		if(lexical_parser_error){
			tigerFile.close();
			return 0;
		}
		
		for (auto token : tokens.getTokens()) {
			std::size_t ruleNum = token->getType();
			if(ruleNum > ruleNames.size()){
				continue;
			}
			std::string rule = ruleNames[ruleNum-1];
			//std::cout << "< " << rule << "," << token->getText() << " >" << std::endl;
		}
		// sequence of token types on successful parsing
		for (auto token : tokens.getTokens()) {
			std::size_t ruleNum = token->getType();
			if(ruleNum > ruleNames.size()){
				continue;
			}
			std::string rule = ruleNames[ruleNum-1];
			//std::cout << rule << " ";
		}
		std::cout << std::endl;
		std::cout << "successful parse" << std::endl;

		tree::ParseTreeWalker walker = tree::ParseTreeWalker::DEFAULT; 
		//std::cout << "start create symbol table" << std::endl;
		DefSymbolTable *st = new DefSymbolTable();
		walker.walk(st, tree);
		if(st->error.size() == 0){
			//std::cout << "start semantic check" << std::endl;
			SemanticCheck *sc = new SemanticCheck(st->globalScope);
			walker.walk(sc, tree);
			if(sc->error.size() == 0){
				std::cout << "successful compile" << std::endl;
				IRGeneration *ir = new IRGeneration(st->globalScope);
				walker.walk(ir, tree);	
				std::vector<std::string> IRcode;
				RearrangeIR(st->globalScope, IRcode);	
				LivenessAnalysis la(st->globalScope);
				printf("start opt register allocation\n");
				OptRegisterAllocation regAllocator(st->globalScope, ir->gFloatVal);				
				//RegisterAllocation regAllocator(st->globalScope, ir->gFloatVal);
				/*for(auto it = regAllocator.regCode.begin(); it != regAllocator.regCode.end(); ++it){
					std::cout << (*it) << std::endl;
				}*/
				CodeGeneration MIPSCode(regAllocator.regCode, ir->gFloatVal);
				ofstream OpenFile("MIPSCode.s");  
				if(OpenFile.fail()){
					std::cout << "open file failed" << std::endl;  
					exit(0);  
				}
				for(auto it = MIPSCode.assembly.begin(); it != MIPSCode.assembly.end(); ++it){
					std::cout << (*it) << std::endl;
					OpenFile << (*it) << "\n";
				}
				OpenFile.close();
			}
			else{
				for(auto it = sc->error.begin(); it != sc->error.end(); ++it){
					std::cout << (*it) << std::endl;
				}
			}
		}
		else{
			for(auto it = st->error.begin(); it != st->error.end(); ++it){
					std::cout << (*it) << std::endl;
			}
		}
		tigerFile.close();
	}
}
