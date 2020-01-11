#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include "antlr4-runtime.h"
#include "tigerBaseListener.h"
#include "tigerParser.h"
#include "scope.cpp"

#ifndef _FRONTENDHELPER
#define _FRONTENDHELPER

void genErrorMessage(std::string message, antlr4::tree::TerminalNode *ctx, std::vector<std::string> &error);
bool tableFind(std::string id, Scope *curScope, Attributes &attr);
std::string getTypeId(tigerParser::TypeIdContext *ctx);
Type getType(tigerParser::TypeContext *ctx, Scope *curScope);
#endif