
// Generated from tiger.g4 by ANTLR 4.7

#pragma once


#include "antlr4-runtime.h"
#include "tigerParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by tigerParser.
 */
class  tigerVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by tigerParser.
   */
    virtual antlrcpp::Any visitProgram(tigerParser::ProgramContext *context) = 0;

    virtual antlrcpp::Any visitDeclarationSegment(tigerParser::DeclarationSegmentContext *context) = 0;

    virtual antlrcpp::Any visitTypeDeclarationList(tigerParser::TypeDeclarationListContext *context) = 0;

    virtual antlrcpp::Any visitVarDeclarationList(tigerParser::VarDeclarationListContext *context) = 0;

    virtual antlrcpp::Any visitFunctionDeclarationList(tigerParser::FunctionDeclarationListContext *context) = 0;

    virtual antlrcpp::Any visitTypeDeclaration(tigerParser::TypeDeclarationContext *context) = 0;

    virtual antlrcpp::Any visitType(tigerParser::TypeContext *context) = 0;

    virtual antlrcpp::Any visitTypeId(tigerParser::TypeIdContext *context) = 0;

    virtual antlrcpp::Any visitVarDeclaration(tigerParser::VarDeclarationContext *context) = 0;

    virtual antlrcpp::Any visitIdList(tigerParser::IdListContext *context) = 0;

    virtual antlrcpp::Any visitOptionalInit(tigerParser::OptionalInitContext *context) = 0;

    virtual antlrcpp::Any visitFunctionDeclaration(tigerParser::FunctionDeclarationContext *context) = 0;

    virtual antlrcpp::Any visitParamList(tigerParser::ParamListContext *context) = 0;

    virtual antlrcpp::Any visitParamListTail(tigerParser::ParamListTailContext *context) = 0;

    virtual antlrcpp::Any visitRetType(tigerParser::RetTypeContext *context) = 0;

    virtual antlrcpp::Any visitParam(tigerParser::ParamContext *context) = 0;

    virtual antlrcpp::Any visitStatSeq(tigerParser::StatSeqContext *context) = 0;

    virtual antlrcpp::Any visitStat(tigerParser::StatContext *context) = 0;

    virtual antlrcpp::Any visitScopeControlFlow(tigerParser::ScopeControlFlowContext *context) = 0;

    virtual antlrcpp::Any visitStatTail(tigerParser::StatTailContext *context) = 0;

    virtual antlrcpp::Any visitLtail(tigerParser::LtailContext *context) = 0;

    virtual antlrcpp::Any visitOptPrefix(tigerParser::OptPrefixContext *context) = 0;

    virtual antlrcpp::Any visitExpr(tigerParser::ExprContext *context) = 0;

    virtual antlrcpp::Any visitConstRule(tigerParser::ConstRuleContext *context) = 0;

    virtual antlrcpp::Any visitExprList(tigerParser::ExprListContext *context) = 0;

    virtual antlrcpp::Any visitExprListTail(tigerParser::ExprListTailContext *context) = 0;

    virtual antlrcpp::Any visitLvalue(tigerParser::LvalueContext *context) = 0;

    virtual antlrcpp::Any visitLvalueTail(tigerParser::LvalueTailContext *context) = 0;


};

