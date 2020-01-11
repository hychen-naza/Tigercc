#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include "antlr4-runtime.h"
#include "tigerBaseListener.h"
#include "tigerParser.h"
#include "FrontEndHelper.h"
#include "scope.cpp"



class IRGeneration : public tigerBaseListener{

	Scope *globalScope;
	Scope *curScope; 
	
	
	int tempNum;
	int labelNum;
	int floatConstNum;
	std::stringstream ss;
	std::map<void *, int> track;

public:
	std::map<std::string, std::string> gFloatVal;
	IRGeneration(Scope *gScope) {
		floatConstNum = 0;
		labelNum = 0;
		tempNum = 0;
        globalScope = gScope;
        curScope = globalScope;
    }

    std::string newTemp(){
    	Attributes attr;
    	while(1){
	    	std::stringstream temp;
	    	temp << "_t" << tempNum;
	    	tempNum++;	
	    	if(tableFind(temp.str(), curScope, attr) == false){
	    		return temp.str();
	    	}
    	}
    }

	std::string newLabel(){
    	std::stringstream label;
    	label << "label" << labelNum;
    	labelNum++;
    	return label.str();
    }
    void enterProgram(tigerParser::ProgramContext *ctx){
    	//curScope->mainCode.push_back("#enterscope");
    	genDeclaration(ctx->declarationSegment());
    	genStatSeq(ctx->statSeq());
    	//curScope->mainCode.push_back("#exitscope");
    }

	void genDeclaration(tigerParser::DeclarationSegmentContext *ctx){
		if(track.find(ctx) == track.end()){
			track.insert(std::pair<void *,int>(ctx, 1));
		}
		else return;

		tigerParser::VarDeclarationListContext *_varDecList = ctx->varDeclarationList();
		while(_varDecList->varDeclaration() != NULL){	
			genVarDec(_varDecList->varDeclaration());
			_varDecList = _varDecList->varDeclarationList();
		}	
		tigerParser::FunctionDeclarationListContext *_funcDecList = ctx->functionDeclarationList();
		while(_funcDecList->functionDeclaration() != NULL){	
			genFuncDec(_funcDecList->functionDeclaration());
			_funcDecList = _funcDecList->functionDeclarationList();
		}		
	}


	void genVarDec(tigerParser::VarDeclarationContext *ctx){
		if(track.find(ctx) == track.end()){
			track.insert(std::pair<void *,int>(ctx, 1));
		}
		else return;		

		if(ctx->optionalInit()->ASSIGN() != NULL){
			tigerParser::IdListContext *_idList = ctx->idList();
			std::string val;
			if(ctx->optionalInit()->constRule()->INTLIT() != NULL){
				val = ctx->optionalInit()->constRule()->INTLIT()->getSymbol()->getText();
			}
			else if(ctx->optionalInit()->constRule()->FLOATLIT() != NULL){
				val = ctx->optionalInit()->constRule()->FLOATLIT()->getSymbol()->getText();
				std::string floatVar = "float_var_" + std::to_string(floatConstNum);
				floatConstNum ++;
				gFloatVal.insert(std::make_pair(val, floatVar));
			}
			bool _isarray = false;
			std::string dimension;
			// array
			if(ctx->type()->ARRAY() != NULL){
				_isarray = true;
				dimension = ctx->type()->INTLIT()->getSymbol()->getText();
			}
			// type alias, and that type is an array
			else if(ctx->type()->ID() != NULL){
				std::string id = ctx->type()->ID()->getSymbol()->getText();

				Attributes attr;
				tableFind(id, curScope, attr);

				if(attr.varType.dimension > 1){
					_isarray = true;
					dimension = std::to_string(attr.varType.dimension);
				}
				else{
					_isarray = false;
					dimension = std::to_string(attr.varType.dimension);
				}
			}
			while(_idList != NULL){
				std::string var = _idList->ID()->getSymbol()->getText();
				ss.str("");
				if(_isarray) ss << "assign, " << var << ", " << dimension << ", " << val;
				else ss << "assign, " << var << ", " << val;
				curScope->mainCode.push_back(ss.str());
				ss.str("");
				_idList = _idList->idList();
			}
		}
		else{
			tigerParser::IdListContext *_idList = ctx->idList();
			while(_idList != NULL){
				std::string var = _idList->ID()->getSymbol()->getText();
				_idList = _idList->idList();
			}
		}
	}

	std::string genFuncParams(Type t){
		if(t.dimension == -1) return "void";   	
    	else if(t.dimension == 1) return t.typeVal;   	
    	else return t.typeVal + "[" + std::to_string(t.dimension) + "]";
	}

	void genFuncDec(tigerParser::FunctionDeclarationContext *ctx){
		if(track.find(ctx) == track.end()){
			track.insert(std::pair<void *,int>(ctx, 1));
		}
		else return;
		std::string func = ctx->ID()->getSymbol()->getText();
		RegInfo *caller = curScope->curReg;
		curScope->curReg = &((curScope->regTable.find(func))->second);
		curScope->curReg->caller = caller;
		Attributes attr;
		tableFind(func, curScope, attr);
    	ss.str("");
    	ss << "#start_function " << func ;
    	curScope->mainCode.push_back(ss.str());
    	ss.str("");
    	//
    	std::string ret;
    	ret = genFuncParams(attr.funcRet);
    	ss << ret << " " << func << "(";
    	for(int i=0; i < attr.funcParams.size(); ++i){
    		if(i == attr.funcParams.size()-1){
    			ss << genFuncParams(attr.funcParams[i].second) << " " << attr.funcParams[i].first;
    		}
    		else{
    			ss << genFuncParams(attr.funcParams[i].second) << " " << attr.funcParams[i].first << ", ";
    		}
    	}
    	ss << ") :";
    	curScope->mainCode.push_back(ss.str());
    	ss.str("");
    	genStatSeq(ctx->statSeq());
    	ss << "#end_function " << func ;
    	curScope->mainCode.push_back(ss.str());
    	ss.str("");
    	std::string instBegin = "#start_function " + func;
    	auto itBegin = find(curScope->mainCode.begin(),curScope->mainCode.end(), instBegin);  	
    	std::string instEnd = "#end_function " + func;
    	auto itEnd = find(curScope->mainCode.begin(),curScope->mainCode.end(), instEnd);   
    	itEnd += 1; 	    	
    	for(auto it = itBegin; it != itEnd; ++it){
    		curScope->funcCode.push_back((*it));
    	}
    	//curScope->funcCode.assign(itBegin, itEnd); // 这样只能def一个function
    	curScope->mainCode.erase(itBegin, itEnd);  
    	// 在globalscope是找不到enterscope的，只有在localscope才有  	
    	if(curScope->mainCode[0].find("#enterscope")==0){
    		auto funcIt = curScope->funcCode.begin();
    		curScope->funcCode.insert(funcIt, curScope->mainCode[0]);
    		curScope->mainCode.erase(curScope->mainCode.begin());
    	}
    	curScope->curReg = curScope->curReg->caller;
	}

	void genStatSeq(tigerParser::StatSeqContext *ctx){
		if(track.find(ctx) == track.end()){
			track.insert(std::pair<void *,int>(ctx, 1));
		}
		else return;
		genStat(ctx->stat());
		if(ctx->statSeq() != NULL) genStatSeq(ctx->statSeq());
	}

	void genStatTail(tigerParser::StatTailContext *ctx){
		if(track.find(ctx) == track.end()){
			track.insert(std::pair<void *,int>(ctx, 1));
		}
		else return;
		if(ctx->ELSE() != NULL){
			genStatSeq(ctx->statSeq());			
		}
	}
	void genScopeControlFlow(tigerParser::ScopeControlFlowContext *ctx){
		if(track.find(ctx) == track.end()){
			track.insert(std::pair<void *,int>(ctx, 1));
		}
		else return;
		if(ctx->FOR() != NULL){
			std::string jumpLabel = newLabel();
			std::string loopLabel = newLabel();
			curScope->loopLabel.first = true;
			curScope->loopLabel.second = jumpLabel;
			std::string id = ctx->ID()->getSymbol()->getText();
	    	std::string t1 = genExpr(ctx->expr(0)); 
    		std::string t2 = genExpr(ctx->expr(1));		 		
    		ss.str(""); 
    		ss << "assign, " << id << ", " << t1;
    		curScope->mainCode.push_back(ss.str());
    		ss.str("");
    		ss << loopLabel << ":";
    		curScope->mainCode.push_back(ss.str());
    		ss.str("");
    		ss << "brgeq, " << id << ", " << t2 << ", " << jumpLabel;
    		curScope->mainCode.push_back(ss.str());
    		ss.str("");
    		genStatSeq(ctx->statSeq());
    		ss.str("");
    		std::string r = newTemp();
    		Type t;
    		t.dimension = 1;
    		t.typeVal = "int";    		
    		curScope->curReg->tempVar.insert(std::make_pair(r, std::make_pair(curScope->curReg->curSize, t)));
    		curScope->curReg->curSize += 4;
    		ss << "add, " << id << ", 1, " << r;
    		curScope->mainCode.push_back(ss.str());
    		ss.str("");
    		ss << "assign, " << id << ", " << r;
    		curScope->mainCode.push_back(ss.str());
    		ss.str("");
			ss << "goto, " << loopLabel;
    		curScope->mainCode.push_back(ss.str());
    		ss.str("");
    		ss << jumpLabel << ":";
    		curScope->mainCode.push_back(ss.str());
    		ss.str("");
		}
		else if(ctx->WHILE() != NULL){
			std::string jumpLabel = newLabel();
			std::string loopLabel = newLabel();
			ss.str("");
			ss << loopLabel << ":";
    		curScope->mainCode.push_back(ss.str());
    		ss.str("");
			curScope->loopLabel.first = true;
			curScope->loopLabel.second = jumpLabel;
			genExprBranch(ctx->expr(0), jumpLabel);
			genStatSeq(ctx->statSeq());
			ss.str("");
			ss << "goto, " << loopLabel;
    		curScope->mainCode.push_back(ss.str());
			ss.str("");
			ss << jumpLabel << ":";
    		curScope->mainCode.push_back(ss.str());
    		ss.str("");
		}
		else if(ctx->IF() != NULL){
			std::string jumpLabel = newLabel();
			// 担心只有一个数字或者字母放在if的判断中，这些情况我都没有处理
			if(ctx->expr(0)->lvalue() == NULL && ctx->expr(0)->constRule() == NULL){
				genExprBranch(ctx->expr(0), jumpLabel);
				genStatSeq(ctx->statSeq());
				std::string elseLabel;
				if(ctx->statTail()->ELSE() != NULL){
					elseLabel = newLabel();
					ss << "goto," << elseLabel;			
					curScope->mainCode.push_back(ss.str());		
					ss.str("");	
				}
				ss.str("");
				ss << jumpLabel << ":";
				curScope->mainCode.push_back(ss.str());
    			ss.str("");    				    			
    			if(ctx->statTail()->ELSE() != NULL){
    				genStatTail(ctx->statTail());
    				ss.str("");
					ss << elseLabel << ":";
					curScope->mainCode.push_back(ss.str());
					ss.str("");
				}
    			
			}
		}
	}

	std::string getScopeNum(){
		Scope *s = curScope;
		std::stringstream localss;
		localss << "#enterscope";
		while(s->parent != NULL){
			localss << "_" << s->whichKid;
			s = s->parent;
		}
		return localss.str();
	}

	void genStat(tigerParser::StatContext * ctx){
		if(track.find(ctx) == track.end()){
			track.insert(std::pair<void *,int>(ctx, 1));
		}
		else return;

		if(ctx->BREAK() != NULL){
			Scope *s = curScope;
			while(s->loopLabel.first != true){
				s = s->parent;
			}
			std::string label = s->loopLabel.second;
			ss.str("");
			ss << "goto, " << label;
			curScope->mainCode.push_back(ss.str());
			ss.str("");
			
		}
		else if(ctx->RETURN() != NULL){
			std::string t1 = genExpr(ctx->expr());
			ss.str("");
			ss << "return, " << t1;
			curScope->mainCode.push_back(ss.str());
			ss.str("");
		}
		else if(ctx->lvalue() != NULL && ctx->ASSIGN() != NULL){
			std::string t1 = genExpr(ctx->expr());
			tigerParser::LvalueContext *_lVal = ctx->lvalue();
			tigerParser::LtailContext *_lTail = ctx->ltail();
			while(1){
				std::string id = _lVal->ID()->getSymbol()->getText();
				if(_lVal->lvalueTail()->expr() != NULL){
					std::string t2 = genExpr(_lVal->lvalueTail()->expr());
					ss.str(""); 
		    		ss << "store, " << id << ", " << t2 << ", " << t1;
		    		curScope->mainCode.push_back(ss.str());
		    		ss.str("");					
				}
				else{
					ss.str(""); 
		    		ss << "assign, " << id << ", " << t1;
		    		curScope->mainCode.push_back(ss.str());
		    		ss.str("");
				}
				if(_lTail->lvalue() != NULL){
					_lVal = _lTail->lvalue();
					_lTail = _lTail->ltail();	
				}	
				else break;
			}
		}
		else if(ctx->LET() != NULL){
			curScope = (curScope->kids.find(ctx))->second;
			std::string scopeNum = getScopeNum();
			curScope->parent->mainCode.push_back(scopeNum);
			//curScope->mainCode.push_back(scopeNum);
			genDeclaration(ctx->declarationSegment());
			genStatSeq(ctx->statSeq());
			curScope->mainCode.push_back("#exitscope");
			curScope = curScope->parent;
		}
		// optPrefix
		else if(ctx->ID() != NULL && ctx->LPAREN() != NULL){
			if(ctx->optPrefix()->lvalue() != NULL){
				Attributes attr;
				auto _lvalue = ctx->optPrefix()->lvalue();
				std::string id = _lvalue->ID()->getSymbol()->getText();
				std::string t1 = newTemp();
				if(tableFind(id, curScope, attr)){
					Type t;
    				t.dimension = 1;
    				t.typeVal = "int";
					if(attr.varType.typeVal.compare("float")==0) t.typeVal = "float";
					curScope->curReg->tempVar.insert(std::make_pair(t1, std::make_pair(curScope->curReg->curSize, t)));
					curScope->curReg->curSize += 4;
				}
				else{
					printf("error in 385 in IRGeneration\n");
				}
			
				ss.str("");
				ss << "callr, " << t1 << ", " << ctx->ID()->getSymbol()->getText();
				if(ctx->exprList()->expr() != NULL){
					std::string t2 = genExpr(ctx->exprList()->expr());
					ss << ", " << t2;
					auto _exprListTail = ctx->exprList()->exprListTail();
					while(_exprListTail->expr() != NULL){
						t2 = genExpr(_exprListTail->expr());
						ss << ", " << t2;
						_exprListTail = _exprListTail->exprListTail();
					}
					curScope->mainCode.push_back(ss.str());
					ss.str("");
				}
							
				if(_lvalue->lvalueTail()->expr() != NULL){
					std::string t2 = genExpr(_lvalue->lvalueTail()->expr());
					ss.str(""); 
		    		ss << "store, " << id << ", " << t2 << ", " << t1;
		    		curScope->mainCode.push_back(ss.str());
		    		ss.str("");	
				}
				else{
					ss.str(""); 
		    		ss << "assign, " << id << ", " << t1;
		    		curScope->mainCode.push_back(ss.str());
		    		ss.str("");
				}
			}
			else{
				if(ctx->exprList()->expr() != NULL){
					std::string t1 = genExpr(ctx->exprList()->expr());
					ss.str("");
					ss << "call, " << ctx->ID()->getSymbol()->getText();
					ss << ", " << t1;
					auto _exprListTail = ctx->exprList()->exprListTail();
					while(_exprListTail->expr() != NULL){
						std::string t1 = genExpr(_exprListTail->expr());
						ss << ", " << t1;
						_exprListTail = _exprListTail->exprListTail();
					}
					
				}
				else{
					ss.str("");
					ss << "call, " << ctx->ID()->getSymbol()->getText();
				}
				curScope->mainCode.push_back(ss.str());
				ss.str("");				
			}
		}
		else if(ctx->scopeControlFlow() != NULL){
			genScopeControlFlow(ctx->scopeControlFlow());
		}
	}



	std::string genExpr(tigerParser::ExprContext * ctx){
		if(track.find(ctx) == track.end()){
			track.insert(std::pair<void *,int>(ctx, 1));
		}
		else return "";

		if(ctx->lvalue() != NULL){
			if(ctx->lvalue()->lvalueTail()->expr() != NULL){
				std::string r = newTemp();
	    		std::string t1 = ctx->lvalue()->ID()->getSymbol()->getText();
	    		Attributes attr;
	    		if(tableFind(t1, curScope, attr)){
					Type t;
    				t.dimension = 1;
    				t.typeVal = "int";
					if(attr.varType.typeVal.compare("float")==0) t.typeVal = "float";
					curScope->curReg->tempVar.insert(std::make_pair(r, std::make_pair(curScope->curReg->curSize, t)));
					curScope->curReg->curSize += 4;
	    		}
	    		std::string t2 = genExpr(ctx->lvalue()->lvalueTail()->expr());
	    		std::stringstream localss;
	    		localss.str("");
	    		localss << "load, " << r << ", " << t1 << ", " << t2;
	    		curScope->mainCode.push_back(localss.str());
	    		localss.str("");   
	    		return r;
			}
			else{
				std::string id = ctx->lvalue()->ID()->getSymbol()->getText();
				auto it = curScope->curReg->defVar.find(id);
				if(it == curScope->curReg->defVar.end()){
					Attributes attr;
					tableFind(id, curScope, attr);
					curScope->curReg->outscopeVar.insert(std::make_pair(id, std::make_pair(curScope->curReg->curSize, attr.varType)));
					curScope->curReg->curSize += 4;
				}
				return id;
			}  					
    	}

    	else if(ctx->constRule() != NULL){
    		if(ctx->constRule()->INTLIT() != NULL) return ctx->constRule()->INTLIT()->getSymbol()->getText();
    		else if(ctx->constRule()->FLOATLIT() != NULL){
    			std::string floatVal = ctx->constRule()->FLOATLIT()->getSymbol()->getText();
    			std::string floatVar = "float_var_" + std::to_string(floatConstNum);
				floatConstNum ++;
				gFloatVal.insert(std::make_pair(floatVal, floatVar));
    			return floatVal;
    		} 
    	}

    	else if(ctx->LPAREN() != NULL){
    		return genExpr(ctx->expr(0));
    	}
    	else{
    		std::string t1 = genExpr(ctx->expr(0)); 
    		std::string t2 = genExpr(ctx->expr(1));
    		std::string r = newTemp();
    		Attributes attr;
    		Type t;
    		t.dimension = 1;
    		t.typeVal = "int";
    		// var, 可以去外层找, 找t1就可以了，t1,t2类型一致
    		if(tableFind(t1, curScope, attr)){
    			if(attr.varType.typeVal.compare("float")==0) t.typeVal = "float";
    		}
    		// const
    		else if(IsFloatNum(t1)) t.typeVal = "float";
    		// 形参
    		else if(curScope->curReg->defVar.find(t1) != curScope->curReg->defVar.end()){
    			auto it = curScope->curReg->defVar.find(t1);
    			if((it->second).second.typeVal.compare("float")==0) t.typeVal = "float";
    		}
    		// _t, 不可以去外层找，不过外层也不可能有就是了
    		else if(curScope->curReg->tempVar.find(t1) != curScope->curReg->tempVar.end()){
    			auto it = curScope->curReg->tempVar.find(t1);
    			if((it->second).second.typeVal.compare("float")==0) t.typeVal = "float";
    		}
    		curScope->curReg->tempVar.insert(std::make_pair(r, std::make_pair(curScope->curReg->curSize, t)));
			curScope->curReg->curSize += 4;
    		std::stringstream localss;
    		localss.str("");
    		if(ctx->MULT() != NULL) localss << "mult, " << t1 << ", " << t2 << ", " << r;
    		else if(ctx->DIV() != NULL) localss << "div, " << t1 << ", " << t2 << ", " << r;
    		else if(ctx->PLUS() != NULL) localss << "add, " << t1 << ", " << t2 << ", " << r;
    		else if(ctx->MINUS() != NULL) localss << "sub, " << t1 << ", " << t2 << ", " << r;
    		else if(ctx->AND() != NULL) localss << "and, " << t1 << ", " << t2 << ", " << r;
    		else if(ctx->OR() != NULL) localss << "or, " << t1 << ", " << t2 << ", " << r;
    		curScope->mainCode.push_back(localss.str());
    		localss.str("");    		
    		return r;
    	}
	}
	bool IsFloatNum(const std::string &str){  
	    bool floatPoint = false;
	    for (int i = 0; i < str.size(); i++){
	        if(str[i]=='.' && floatPoint == false){
	            floatPoint = true;
	            continue;
	        }
	        else if(str[i]=='.' && floatPoint == true){
	        	return false;
	        }
	        int tmp = (int)str[i];
	        if (tmp >= 48 && tmp <= 57) continue;
	        else return false;
	    } 
	    if(floatPoint) return true;
	}
	std::string genExprBranch(tigerParser::ExprContext * ctx, std::string jumpLabel){
		if(track.find(ctx) == track.end()){
			track.insert(std::pair<void *,int>(ctx, 1));
		}
		else return "";

		if(ctx->lvalue() != NULL){
    		return ctx->lvalue()->ID()->getSymbol()->getText();
    	}
    	else if(ctx->constRule() != NULL){
    		if(ctx->constRule()->INTLIT() != NULL) return ctx->constRule()->INTLIT()->getSymbol()->getText();
    		else if(ctx->constRule()->FLOATLIT() != NULL) return ctx->constRule()->FLOATLIT()->getSymbol()->getText();
    	}
    	else if(ctx->LPAREN() != NULL){
    		return genExprBranch(ctx->expr(0), jumpLabel);
    	}

    	else{
    		std::string t1 = genExpr(ctx->expr(0)); 
    		std::string t2 = genExpr(ctx->expr(1));
    		std::stringstream localss;
    		localss.str("");
    		if(ctx->EQ() != NULL) localss << "brneq, " << t1 << ", " << t2 << ", " << jumpLabel;
    		else if(ctx->NEQ() != NULL) localss << "breq, " << t1 << ", " << t2 << ", " << jumpLabel;
    		else if(ctx->GREATEREQ() != NULL) localss << "brlt, " << t1 << ", " << t2 << ", " << jumpLabel;
    		else if(ctx->LESSEREQ() != NULL) localss << "brgt, " << t1 << ", " << t2 << ", " << jumpLabel;
    		else if(ctx->GREATER() != NULL) localss << "brleq, " << t1 << ", " << t2 << ", " << jumpLabel;
    		else if(ctx->LESSER() != NULL) localss << "brgeq, " << t1 << ", " << t2 << ", " << jumpLabel;
    		curScope->mainCode.push_back(localss.str());
    		localss.str("");
    		return "";
    	}
	}
};


