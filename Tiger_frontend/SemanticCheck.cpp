#include <map>
#include <string>
#include <algorithm>
#include "antlr4-runtime.h"
#include "tigerBaseListener.h"
#include "tigerParser.h"
#include "scope.cpp"



class SemanticCheck : public tigerBaseListener{

	Scope *globalScope;
	Scope *curScope; 
	std::stringstream ss;

public:
	std::vector<std::string> error;

	SemanticCheck(Scope *gScope) {
        globalScope = gScope;
        curScope = globalScope;
    }

	void genErrorMessage(std::string message, antlr4::tree::TerminalNode *ctx){
		ss.str("");
		ss << "Error: line " << ctx->getSymbol()->getLine() << ":" << ctx->getSymbol()->getCharPositionInLine() << ", " << message;
		error.push_back(ss.str());
		ss.str("");
	}

    std::string getTypeId(tigerParser::TypeIdContext *ctx){
		if(ctx->INT() != NULL){
			return "int";
		}
		else{
			return "float";
		}
	}


	Type getType(tigerParser::TypeContext *ctx){
		Type t;
		if(ctx->ID() != NULL){
			Attributes attr;
			tableFind(ctx->ID()->getSymbol()->getText(), attr);			
			return attr.varType;
		}

		else if(ctx->ARRAY() != NULL){
			std::stringstream tem(ctx->INTLIT()->getSymbol()->getText());
			tem >> t.dimension;
			t.typeVal = getTypeId(ctx->typeId());
			return t;
		}

		else if(ctx->ARRAY() == NULL && ctx->typeId() != NULL){
			t.dimension = 1;
			t.typeVal = getTypeId(ctx->typeId());
			return t;
		}
		else{
			printf("error, you shouldn't come here in getType()\n");
		}
	}

    void enterType(tigerParser::TypeContext *ctx){
    	if(ctx->ID() != NULL){
    		std::string id = ctx->ID()->getSymbol()->getText();
    		Attributes attr;
    		if(!tableFind(id, attr)){
    			std::string message = id + " not declared.";
    			genErrorMessage(message, ctx->ID());
    		}
    		else if(attr.type.compare("type") != 0){
    			std::string message = id + " is not a type name.";
    			genErrorMessage(message, ctx->ID());
    		}
    	}    	
    }

    bool checkFuncParamType(Type t1, Type t2){
    	if(t1.dimension != t2.dimension || t1.typeVal.compare(t2.typeVal) != 0){
    		return false;
    	}
    	else return true;
    }

	class isParamEqual {
	public:
	    explicit isParamEqual(std::string param) : parameter(param) {}
	    bool operator() (const std::pair<std::string, Type>& element) const {
	        if(element.first.compare(parameter)==0) return true;
	        else return false;
	    }
	private:
	    const std::string parameter;
	};
    bool tableFind(std::string id, Attributes &attr){
    	Scope *s = curScope;
    	if(curScope->withinFunc.first){
    		std::string func = curScope->withinFunc.second;
    		// 不需要再去outscope找了，这个是在func declare时用的，如果这找不到，说明func name undefined, 而这不可能
    		if(s->table.find(func) != s->table.end()){
    			std::vector<std::pair<std::string, Type> > vec = ((s->table.find(func))->second).funcParams;
    			auto it = std::find_if(vec.begin(), vec.end(), isParamEqual(id));
    			if(it != vec.end()){
    				attr.varType.dimension = it->second.dimension;
    				attr.varType.typeVal = it->second.typeVal;
    				attr.type = "var";
    				return true;
    			}
    		}
    	}

    	while(s != NULL){
    		if(s->table.find(id) != s->table.end()){
    			attr = (s->table.find(id))->second;
    			return true;
    		}
    		else{
    			s = s->parent;
    		}
    	}
    	return false;
    }


    void checkAssign(tigerParser::LvalueContext *ctx, Type t){
    	// dimension check
    	if(t.dimension != 1){
    		std::string message = "rvalue is an array, further (not init) aggregate assignmen not allowed.";
    		genErrorMessage(message, ctx->ID());
    	}
    	else if(t.dimension == 1){
    		std::string id = ctx->ID()->getSymbol()->getText();
    		Attributes attr;
    		if(!tableFind(id, attr)){
    			std::string message = id + " not declared.";
    			genErrorMessage(message, ctx->ID());
    		}
    		else{
    			if(attr.varType.dimension > 1 && ctx->lvalueTail()->expr() == NULL){
    				std::string message = id + " is an array, further (not init) aggregate assignmen not allowed.";
    				genErrorMessage(message, ctx->ID());
    			}    
    			else if(attr.varType.dimension == 1){
    				if(attr.varType.typeVal.compare("int") == 0 && t.typeVal.compare("float") == 0){
						std::string message = "assign float value to int var.";
						genErrorMessage(message, ctx->ID());
					}				
    			}		
    		}
    	}
    }

    void enterStat(tigerParser::StatContext *ctx){
    	// RETURN的情况在st时就check过了
    	if(ctx->LET() != NULL){
    		curScope = (curScope->kids.find(ctx))->second;

    	}
    	else if(ctx->ASSIGN() != NULL){
    		Type t = genExprType(ctx->expr());
    		checkAssign(ctx->lvalue(), t);
    		tigerParser::LtailContext *_ltail = ctx->ltail();
    		while(_ltail->lvalue() != NULL){
    			checkAssign(_ltail->lvalue(), t);
    			_ltail = _ltail->ltail();
    		} 
    	}
    	else if(ctx->BREAK() != NULL){
    		if(curScope->loopLabel.first != true){
    			std::string message = "break not within loop.";
    			genErrorMessage(message, ctx->BREAK());
    		}
    	}
    	else if(ctx->ID() != NULL && ctx->LPAREN() != NULL){
    		std::string func = ctx->ID()->getSymbol()->getText();
    		Attributes attr;
    		if(!tableFind(func, attr)){
    			std::string message = "function " + func + " is not declared.";
    			genErrorMessage(message, ctx->ID());
    			return;
    		}
    		else{
    			if(attr.type.compare("function") != 0){
    				std::string message = func + " is not a function name.";
    				genErrorMessage(message, ctx->ID());
	    			return;
    			}
    		}
    		//st 中已经确认要么ret type function 有return statement, 要么void type function 没return statement
    		//还有两种错误， void type function有左赋值； ret type function的左赋值type不对
    		if(ctx->optPrefix()->lvalue() != NULL){
    			if(attr.funcRet.dimension == -1){
    				std::string message = "void function is assigned to lvalue.";
    				genErrorMessage(message, ctx->ID());
    				return;    				
    			}
    			else{
    				checkAssign(ctx->optPrefix()->lvalue(), attr.funcRet);   				
    			}
    		}
    		int paramNum = 0;
    		if(ctx->exprList()->expr() != NULL){
    			paramNum = 1;
    			auto it = ctx->exprList()->exprListTail();
    			while(it->expr() != NULL){
    				paramNum += 1;
    				it = it->exprListTail();
    			}
    		}

    		if(paramNum != attr.funcParams.size()){
    			std::string message = "function parameter number is not matched, should be " + std::to_string(attr.funcParams.size()) + \
    				" parameters, but " + std::to_string(paramNum) + " arguments provided.";
				genErrorMessage(message, ctx->ID());
				return;    	
    		}
    		
    		else{
    			// check param type
    			paramNum = 0;
    			if(ctx->exprList()->expr() != NULL){
    				Type _exprtype = genExprType(ctx->exprList()->expr());
    				if(!checkFuncParamType(_exprtype, attr.funcParams[paramNum].second)){
    					ss.str("");
    					ss << "Error : line " << ctx->ID()->getSymbol()->getLine() << ":" << ctx->ID()->getSymbol()->getCharPositionInLine() << " , function's " << paramNum << " parameter is not matched. ";
    					ss << "should be type " << attr.funcParams[paramNum].second.typeVal << " with length " << attr.funcParams[paramNum].second.dimension;
    					ss << " but provied type " << _exprtype.typeVal << " with length " << _exprtype.dimension;
    					error.push_back(ss.str());
    					ss.str("");
    				}

	    			paramNum += 1;
	    			auto it = ctx->exprList()->exprListTail();
	    			while(it->expr() != NULL){
	    				Type _exprtype = genExprType(ctx->exprList()->expr());
    					if(!checkFuncParamType(_exprtype, attr.funcParams[paramNum].second)){
    						ss.str("");
    						ss << "Error : line " << ctx->ID()->getSymbol()->getLine() << ":" << ctx->ID()->getSymbol()->getCharPositionInLine() << " , function's " << paramNum << " parameter is not matched. ";
	    					ss << "should be type " << attr.funcParams[paramNum].second.typeVal << " with length " << attr.funcParams[paramNum].second.dimension;
	    					ss << " but provied type " << _exprtype.typeVal << " with length " << _exprtype.dimension;
	    					error.push_back(ss.str());
	    					ss.str("");
    					}
	    				paramNum += 1;
	    				it = it->exprListTail();
	    			}
	    		}	    		
    		}
    	}
    }

    void enterScopeControlFlow(tigerParser::ScopeControlFlowContext *ctx){
    	if(ctx->FOR() != NULL){
    		curScope->loopLabel.first = true;
    		std::string id = ctx->ID()->getSymbol()->getText();
    		Attributes attr;
    		if(!tableFind(id, attr)){
    			std::string message = id + " not declared.";
    			genErrorMessage(message, ctx->ID());
    		}
    		else if(attr.type.compare("var") != 0){
    			std::string message = id + " is not a var name.";
    			genErrorMessage(message, ctx->ID());
    		}
    	}
    	else if(ctx->WHILE() != NULL){
    		curScope->loopLabel.first = true;
    	}
    }

    void exitScopeControlFlow(tigerParser::ScopeControlFlowContext *ctx){
    	if(ctx->FOR() != NULL){
    		curScope->loopLabel.first = false;
    	}
    	else if(ctx->WHILE() != NULL){
    		curScope->loopLabel.first = false;
    	}
    }

    void exitStat(tigerParser::StatContext *ctx){
    	if(ctx->LET() != NULL){
    		curScope = curScope->parent;
    	}
    }

    Type genExprType(tigerParser::ExprContext *ctx){
    	if(ctx->lvalue() != NULL){
    		std::string id = ctx->lvalue()->ID()->getSymbol()->getText();

    		Attributes attr;
    		bool iffind = tableFind(id, attr);    		
    		if(iffind){
    			if(attr.type.compare("var") != 0){
    				std::string message = id + " is not a variable name.";
    				genErrorMessage(message, ctx->lvalue()->ID());
    			}
    			// id是array，但没有加[下标]; 此处数组越界的情况不检查，因为要计算了
    			else if(attr.varType.dimension > 1 && ctx->lvalue()->lvalueTail()->expr() == NULL){
    				std::string message = id + " is an array, aggregate operations not allowed.";
    				genErrorMessage(message, ctx->lvalue()->ID());   				  				
    			}
    			else if(attr.varType.dimension > 1 && ctx->lvalue()->lvalueTail()->expr() != NULL){
    				Type t;
    				t.typeVal = attr.varType.typeVal;
    				t.dimension = 1;
    				return t;  
    			}
    		} 
    		return attr.varType;				
    	}

    	else if(ctx->constRule() != NULL){
    		Type t;
    		t.dimension = 1;
    		if(ctx->constRule()->INTLIT() != NULL){   			
    			t.typeVal = "int";
    		} 
    		else if(ctx->constRule()->FLOATLIT() != NULL){
    			t.typeVal = "float";    		 
    		}
    		return t;
    	}

    	else if(ctx->LPAREN() != NULL){
    		return genExprType(ctx->expr(0));
    	}

    	else{
    		Type t1 = genExprType(ctx->expr(0)); 
    		Type t2 = genExprType(ctx->expr(1));    
    		if(t1.dimension == 1 && t2.dimension == 1){   			
    			Type t;
    			t.dimension = 1; // since array op not allowed, if we detect array in t1,t2, then error
    			// ** error detection
    			if(ctx->EXP() == NULL){				
    				if(t1.typeVal.compare(t2.typeVal) != 0){
    					antlr4::tree::TerminalNode *binaryop;
			    		if(ctx->MULT() != NULL) binaryop = ctx->MULT();
			    		else if(ctx->DIV() != NULL) binaryop = ctx->DIV();
			    		else if(ctx->PLUS() != NULL) binaryop = ctx->PLUS();
			    		else if(ctx->MINUS() != NULL) binaryop = ctx->MINUS();
			    		else if(ctx->EQ() != NULL) binaryop = ctx->EQ();
			    		else if(ctx->NEQ() != NULL) binaryop = ctx->NEQ();
			    		else if(ctx->GREATER() != NULL) binaryop = ctx->GREATER();
			    		else if(ctx->LESSER() != NULL) binaryop = ctx->LESSER();
			    		else if(ctx->GREATEREQ() != NULL) binaryop = ctx->GREATEREQ();
			    		else if(ctx->LESSEREQ() != NULL) binaryop = ctx->LESSEREQ();
			    		else if(ctx->AND() != NULL) binaryop = ctx->AND();
			    		else if(ctx->OR() != NULL) binaryop = ctx->OR();
    					std::string message = "operators' types are not identical, first is "+t1.typeVal+", second is "+t2.typeVal+".";
    					genErrorMessage(message, binaryop);
    					t.typeVal = "int";
    				}
    				else t.typeVal = t1.typeVal;
    				return t;
    			}
    			else{
    				if(t2.typeVal.compare("int") == 0){
    					if(t1.typeVal.compare("int") != 0){
    						std::string message = "operators' types are not identical, first is "+t1.typeVal+", second is "+t2.typeVal+".";
    						genErrorMessage(message, ctx->EXP());
    					}
    					t.typeVal = "int";
    					return t;
    				}
    				// error, exp a**b, b must be a int
    				else{
    					std::string message = "second operator in ** is not a int.";
    					genErrorMessage(message, ctx->EXP());
				    	t.typeVal = "int"; // cover this error in exp
    					return t;
    				}
    			}
    		}
    		else if(t1.dimension > 1 || t2.dimension > 1){
    			antlr4::tree::TerminalNode *binaryop;
    			if(ctx->EXP() != NULL) binaryop = ctx->EXP();
	    		else if(ctx->MULT() != NULL) binaryop = ctx->MULT();
	    		else if(ctx->DIV() != NULL) binaryop = ctx->DIV();
	    		else if(ctx->PLUS() != NULL) binaryop = ctx->PLUS();
	    		else if(ctx->MINUS() != NULL) binaryop = ctx->MINUS();
	    		else if(ctx->EQ() != NULL) binaryop = ctx->EQ();
	    		else if(ctx->NEQ() != NULL) binaryop = ctx->NEQ();
	    		else if(ctx->GREATER() != NULL) binaryop = ctx->GREATER();
	    		else if(ctx->LESSER() != NULL) binaryop = ctx->LESSER();
	    		else if(ctx->GREATEREQ() != NULL) binaryop = ctx->GREATEREQ();
	    		else if(ctx->LESSEREQ() != NULL) binaryop = ctx->LESSEREQ();
	    		else if(ctx->AND() != NULL) binaryop = ctx->AND();
	    		else if(ctx->OR() != NULL) binaryop = ctx->OR();
	    		std::string message = "no aggregate operations are allowed.";
    			genErrorMessage(message, binaryop);    			
		    	Type t;
		    	// we have to assign a proper dimension, use array length can report the aggregate problem up recursively
		    	t.dimension = std::max(t1.dimension, t2.dimension); 
		    	return t;
    		}
    		else{
    			printf("in genExprType should not come here\n");
    		}
    	}
    }


    void enterExpr(tigerParser::ExprContext *ctx){
    	// == error detection
    	if(ctx->EQ() != NULL){
			if(EQFind(ctx->expr(0)) | EQFind(ctx->expr(1))){
				std::string message = "comparison(==) operators do not associate.";
				genErrorMessage(message, ctx->EQ());
			}   				
		}
    }        			
    			

    bool EQFind(tigerParser::ExprContext *ctx){
    	if(ctx->LPAREN() != NULL){
    		return EQFind(ctx->expr(0));
    	}
    	else if(ctx->lvalue()==NULL && ctx->constRule()==NULL){
    		if(ctx->EQ() != NULL) return true;
    		else return false;
    	}
    	else return false;
    }


    void enterLvalue(tigerParser::LvalueContext *ctx){
    	Attributes attr;
    	std::string id = ctx->ID()->getSymbol()->getText();
    	if(!tableFind(id, attr)){
    		std::string message = id + " is not declared";
    		genErrorMessage(message, ctx->ID());
    	}
    	else if(attr.type.compare("var") != 0){
    		std::string message = id + " is not a variable name";
    		genErrorMessage(message, ctx->ID());
    	}
    	else{
    		// 关于array indexer不为Int的错误
    		if(ctx->lvalueTail()->expr() != NULL){
	    		Type t = genExprType(ctx->lvalueTail()->expr());
	    		if(t.typeVal.compare("int") != 0){
	    			std::string message = "array indexer is not int value.";
	    			genErrorMessage(message, ctx->ID());
	    		}
	    	}
    	}
    }

	void enterVarDeclaration(tigerParser::VarDeclarationContext *ctx){
		// after st, it is a valid id (no duplicate define)
		if(ctx->optionalInit()->constRule() != NULL){
			Type t = getType(ctx->type());
			//assign float value to int var
			if(t.typeVal.compare("int") == 0 && ctx->optionalInit()->constRule()->FLOATLIT() != NULL){
				std::string message = "assign float value to int var.";
				genErrorMessage(message, ctx->optionalInit()->constRule()->FLOATLIT());
			}
		}
	}

	void enterFunctionDeclaration(tigerParser::FunctionDeclarationContext *ctx){
		curScope->withinFunc.first = true;
		std::string func = ctx->ID()->getSymbol()->getText();
		curScope->withinFunc.second = func;
	}

	void exitFunctionDeclaration(tigerParser::FunctionDeclarationContext *ctx){
		curScope->withinFunc.first = false;
	}

};


