#include "FrontEndHelper.h"



void genErrorMessage(std::string message, antlr4::tree::TerminalNode *ctx, std::vector<std::string> &error){
	std::stringstream ss;
	ss.str("");
	ss << "Error: line " << ctx->getSymbol()->getLine() << ":" << ctx->getSymbol()->getCharPositionInLine() << ", " << message;
	error.push_back(ss.str());
	ss.str("");
}


bool tableFind(std::string id, Scope *curScope, Attributes &attr){
	Scope *s = curScope;
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

std::string getTypeId(tigerParser::TypeIdContext *ctx){
	if(ctx->INT() != NULL){
		return "int";
	}
	else{
		return "float";
	}
}


Type getType(tigerParser::TypeContext *ctx, Scope *curScope){
	Type t;
	if(ctx->ID() != NULL){
		std::string id = ctx->ID()->getSymbol()->getText();
		Scope *s = curScope;
		while(s != NULL){
			if(s->table.find(id) != s->table.end()){
				Attributes attr = (s->table.find(id))->second;
				return attr.varType;
			}
			else{
				s = s->parent;
			}
		}
		std::string message = id + " is not a type.";
		std::stringstream ss;
		ss.str("");
		ss << "Error: line " << ctx->ID()->getSymbol()->getLine() << ":" << ctx->ID()->getSymbol()->getCharPositionInLine() << ", " << message;
		std::cout << ss.str() << std::endl;
		ss.str("");
		return t;								
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

