#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include "antlr4-runtime.h"
#include "tigerBaseListener.h"
#include "tigerParser.h"
#include "FrontEndHelper.h"
#include "scope.cpp"

class DefSymbolTable : public tigerBaseListener{

	std::stringstream ss;
	int scopenum = 1;	
	Scope *curScope; 

public:

	Scope *globalScope;
	std::vector<std::string> error;

    void addStandLibPrinti(){
    	Attributes attr;
    	Type t;
    	attr.type = "function";
    	attr.funcRet.dimension = -1;
    	t.dimension = 1;
    	t.typeVal = "int";
    	attr.funcParams.push_back(std::make_pair("i", t));
    	globalScope->table.insert(std::pair<std::string,Attributes>("printi", attr));
    }
    void addStandLibFlush(){
    	Attributes attr;
    	attr.type = "function";
    	attr.funcRet.dimension = -1;
    	globalScope->table.insert(std::pair<std::string,Attributes>("flush", attr));
    }  
    void addStandLib(){
    	addStandLibPrinti();
    	addStandLibFlush();
    }

	void enterProgram(tigerParser::ProgramContext *ctx){
		globalScope = new Scope();
		globalScope->parent = NULL;
		globalScope->loopLabel.first = false;
		globalScope->whichKid = 0;
		RegInfo rinfo;
		rinfo.curSize = 0;
		rinfo.caller = NULL;
		rinfo.retVal.dimension = -1;
		globalScope->regTable.insert(std::pair<std::string, RegInfo>("main", rinfo));	
		globalScope->curReg = &((globalScope->regTable.find("main"))->second);
		addStandLib();
		curScope = globalScope;
	}

	void exitProgram(tigerParser::ProgramContext *ctx){		
		int tabnum = 0;
		if(error.size() == 0){
			Scope *s = globalScope;
			outputScopes(s, tabnum);
		}			
	}

	void outputTab(int tabnum){
		for(int i=0;i<tabnum;++i) std::cout << "\t";
	}

	void outputScopes(Scope *s, int tabnum){
		//std::cout << "kids num : " << s->kids.size() << std::endl;
		outputTab(tabnum);
		std::cout << "scope " << scopenum << ":" << std::endl;
		for(auto it = s->table.begin(); it != s->table.end(); ++it){
			if(it->second.type.compare("function") != 0){
				outputTab(tabnum+1);
				if(it->second.varType.dimension > 1){
					if(it->second.varType.typeVal.compare("int")==0 || it->second.varType.typeVal.compare("float")==0){
						std::cout << it->first << ", " << it->second.type << ", " << it->second.varType.typeVal << "["<<it->second.varType.dimension<<"]" << std::endl;
					}
					else{
						//如果是array的别名就不输出dimension
						std::cout << "it should not come here in symbol table 152" << std::endl;
						std::cout << it->first << ", " << it->second.type << ", " << it->second.varType.typeVal << std::endl;
					}					
				}
				else{
					std::cout << it->first << ", " << it->second.type << ", " << it->second.varType.typeVal << std::endl;
				}
				
			}
			else{
				if(it->first.compare("flush")==0 || it->first.compare("getchar")==0 || it->first.compare("printi")==0 || it->first.compare("prints")==0){
					continue;
				}
				if(it->second.funcRet.dimension == -1){
					outputTab(tabnum+1);
					std::cout << it->first << ", func, null" << std::endl;
				}
				else{
					outputTab(tabnum+1);
					std::cout << it->first << ", func, " << it->second.funcRet.typeVal << std::endl;
				}
			}
		}
		
		for(auto it = s->kids.begin(); it != s->kids.end(); ++it){
			scopenum += 1;
			int newtabnum = tabnum + 1;
			outputScopes(it->second, newtabnum);
		}
	}

    void createNewScope(tigerParser::StatContext *ctx){
		Scope *scope = new Scope();
		scope->parent = curScope;
		scope->withinFunc.first = false;
		scope->whichKid = curScope->scopeKids.size();
		scope->loopLabel.first = false;
		RegInfo rinfo;
		rinfo.curSize = 0;
		rinfo.caller = NULL;
		rinfo.retVal.dimension = -1;
		scope->regTable.insert(std::pair<std::string, RegInfo>("main", rinfo));
		scope->curReg = &((scope->regTable.find("main"))->second);
		curScope->kids.insert(std::pair<tigerParser::StatContext *,Scope *>(ctx, scope));
		curScope->scopeKids.push_back(scope);
		curScope = scope;
	}

	void enterStat(tigerParser::StatContext * ctx){
		if(ctx->LET() != NULL){
			createNewScope(ctx);
		}
	}

	void exitStat(tigerParser::StatContext * ctx){
		if(ctx->LET() != NULL){
			curScope = curScope->parent;
		}
	}

	bool findReturn(tigerParser::FunctionDeclarationContext *ctx){
		auto it = ctx->statSeq();
		while(it != NULL){
			if(it->stat()->RETURN() != NULL){
				return true;
			}
			it = it->statSeq();
		}
		return false;
	}

	void enterFunctionDeclaration(tigerParser::FunctionDeclarationContext *ctx){

		std::string id = ctx->ID()->getSymbol()->getText(); 
		if(curScope->table.find(id) != curScope->table.end()){
			std::string mess = "function name " + id + " has been used, duplicated declaration.";
			genErrorMessage(mess, ctx->ID(), error);			
		}
		Attributes attr;
		attr.type = "function";
		RegInfo *caller = curScope->curReg;
		RegInfo rinfo;
		rinfo.curSize = 0;
		rinfo.caller = caller;
		if(ctx->retType()->type() != NULL){
			attr.funcRet = getType(ctx->retType()->type(), curScope);	
			rinfo.retVal = attr.funcRet;		
		}
		else{
			attr.funcRet.dimension = -1;
			rinfo.retVal.dimension = -1;
		}

		if(attr.funcRet.dimension == -1 && findReturn(ctx) ==  true){
			std::string mess = "no return type function " + id + " has a return statement in it.";
			genErrorMessage(mess, ctx->ID(), error);
		}

		if(attr.funcRet.dimension != -1 && findReturn(ctx) ==  false){
			std::string mess = "return type function " + id + " has no return statement in it.";
			genErrorMessage(mess, ctx->ID(), error);
		}

		if(ctx->paramList()->param() != NULL){
			std::pair<std::string, Type> param(ctx->paramList()->param()->ID()->getSymbol()->getText(), getType(ctx->paramList()->param()->type(), curScope));
			rinfo.defVar.insert(std::make_pair(param.first, std::make_pair(rinfo.curSize, param.second)));
			rinfo.curSize += 4;
			attr.funcParams.push_back(param);						
			tigerParser::ParamListTailContext *_paraListTail = ctx->paramList()->paramListTail();
			while(_paraListTail->param() != NULL){
				std::pair<std::string, Type> param(_paraListTail->param()->ID()->getSymbol()->getText(), getType(_paraListTail->param()->type(), curScope));
				rinfo.defVar.insert(std::make_pair(param.first, std::make_pair(rinfo.curSize, param.second)));
				rinfo.curSize += 4;
				attr.funcParams.push_back(param);
				_paraListTail = _paraListTail->paramListTail();
			}
		}
		curScope->regTable.insert(std::pair<std::string, RegInfo>(id, rinfo));
		curScope->curReg = &((curScope->regTable.find(id))->second);
		curScope->table.insert(std::pair<std::string, Attributes>(id, attr));
	}

	void exitFunctionDeclaration(tigerParser::FunctionDeclarationContext *ctx){
		curScope->curReg = curScope->curReg->caller;
	}
	void enterTypeDeclaration(tigerParser::TypeDeclarationContext *ctx){
		std::string id = ctx->ID()->getSymbol()->getText(); 
		Attributes attr;
		attr.type = "type";
		Type t;
		if(ctx->type()->ID() != NULL){
			std::string _typename = ctx->type()->ID()->getSymbol()->getText();
			Attributes attr;
			if(tableFind(_typename, curScope, attr)){
				t = attr.varType;
			}
			else{
				std::string mess = "type name " + _typename + " is not defined.";
				genErrorMessage(mess, ctx->type()->ID(), error);
			}		
		}

		else if(ctx->type()->ARRAY() != NULL){
			if(ctx->type()->INTLIT() != NULL){
				std::stringstream tem(ctx->type()->INTLIT()->getSymbol()->getText());				
				tem >> t.dimension;
			}			
			t.typeVal = getTypeId(ctx->type()->typeId());
		}

		else if(ctx->type()->ARRAY() == NULL && ctx->type()->typeId() != NULL){
			t.dimension = 1;
			t.typeVal = getTypeId(ctx->type()->typeId());
		}
		attr.varType = t;
		if(curScope->table.find(id) != curScope->table.end()){
			std::string mess = "type name " + id + " has been used, duplicated declaration.";
			genErrorMessage(mess, ctx->ID(), error);			
			return;
		}
		curScope->table.insert(std::pair<std::string,Attributes>(id, attr));
	}

	void enterVarDeclaration(tigerParser::VarDeclarationContext * ctx){
		Attributes attr;
		attr.type = "var";
		attr.varType = getType(ctx->type(), curScope);
		tigerParser::IdListContext *_idListCtx = ctx->idList();
		while(_idListCtx != NULL){
			std::string id = _idListCtx->ID()->getSymbol()->getText(); 
			curScope->curReg->defVar.insert(std::make_pair(id, std::make_pair(curScope->curReg->curSize, attr.varType)));
			curScope->curReg->curSize += attr.varType.dimension * 4;
			if(curScope->table.find(id) != curScope->table.end()){
				std::string mess = "var name " + id + " has been used, duplicated declaration.";
				genErrorMessage(mess, _idListCtx->ID(), error);					
				_idListCtx = _idListCtx->idList();
				continue;
			}			
			curScope->table.insert(std::pair<std::string, Attributes>(id, attr));
			_idListCtx = _idListCtx->idList();
		}	
	}
};
