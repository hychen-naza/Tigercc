
// Generated from tiger.g4 by ANTLR 4.7

#pragma once


#include "antlr4-runtime.h"
#include "tigerParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by tigerParser.
 */
class  tigerListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProgram(tigerParser::ProgramContext *ctx) = 0;
  virtual void exitProgram(tigerParser::ProgramContext *ctx) = 0;

  virtual void enterDeclarationSegment(tigerParser::DeclarationSegmentContext *ctx) = 0;
  virtual void exitDeclarationSegment(tigerParser::DeclarationSegmentContext *ctx) = 0;

  virtual void enterTypeDeclarationList(tigerParser::TypeDeclarationListContext *ctx) = 0;
  virtual void exitTypeDeclarationList(tigerParser::TypeDeclarationListContext *ctx) = 0;

  virtual void enterVarDeclarationList(tigerParser::VarDeclarationListContext *ctx) = 0;
  virtual void exitVarDeclarationList(tigerParser::VarDeclarationListContext *ctx) = 0;

  virtual void enterFunctionDeclarationList(tigerParser::FunctionDeclarationListContext *ctx) = 0;
  virtual void exitFunctionDeclarationList(tigerParser::FunctionDeclarationListContext *ctx) = 0;

  virtual void enterTypeDeclaration(tigerParser::TypeDeclarationContext *ctx) = 0;
  virtual void exitTypeDeclaration(tigerParser::TypeDeclarationContext *ctx) = 0;

  virtual void enterType(tigerParser::TypeContext *ctx) = 0;
  virtual void exitType(tigerParser::TypeContext *ctx) = 0;

  virtual void enterTypeId(tigerParser::TypeIdContext *ctx) = 0;
  virtual void exitTypeId(tigerParser::TypeIdContext *ctx) = 0;

  virtual void enterVarDeclaration(tigerParser::VarDeclarationContext *ctx) = 0;
  virtual void exitVarDeclaration(tigerParser::VarDeclarationContext *ctx) = 0;

  virtual void enterIdList(tigerParser::IdListContext *ctx) = 0;
  virtual void exitIdList(tigerParser::IdListContext *ctx) = 0;

  virtual void enterOptionalInit(tigerParser::OptionalInitContext *ctx) = 0;
  virtual void exitOptionalInit(tigerParser::OptionalInitContext *ctx) = 0;

  virtual void enterFunctionDeclaration(tigerParser::FunctionDeclarationContext *ctx) = 0;
  virtual void exitFunctionDeclaration(tigerParser::FunctionDeclarationContext *ctx) = 0;

  virtual void enterParamList(tigerParser::ParamListContext *ctx) = 0;
  virtual void exitParamList(tigerParser::ParamListContext *ctx) = 0;

  virtual void enterParamListTail(tigerParser::ParamListTailContext *ctx) = 0;
  virtual void exitParamListTail(tigerParser::ParamListTailContext *ctx) = 0;

  virtual void enterRetType(tigerParser::RetTypeContext *ctx) = 0;
  virtual void exitRetType(tigerParser::RetTypeContext *ctx) = 0;

  virtual void enterParam(tigerParser::ParamContext *ctx) = 0;
  virtual void exitParam(tigerParser::ParamContext *ctx) = 0;

  virtual void enterStatSeq(tigerParser::StatSeqContext *ctx) = 0;
  virtual void exitStatSeq(tigerParser::StatSeqContext *ctx) = 0;

  virtual void enterStat(tigerParser::StatContext *ctx) = 0;
  virtual void exitStat(tigerParser::StatContext *ctx) = 0;

  virtual void enterScopeControlFlow(tigerParser::ScopeControlFlowContext *ctx) = 0;
  virtual void exitScopeControlFlow(tigerParser::ScopeControlFlowContext *ctx) = 0;

  virtual void enterStatTail(tigerParser::StatTailContext *ctx) = 0;
  virtual void exitStatTail(tigerParser::StatTailContext *ctx) = 0;

  virtual void enterLtail(tigerParser::LtailContext *ctx) = 0;
  virtual void exitLtail(tigerParser::LtailContext *ctx) = 0;

  virtual void enterOptPrefix(tigerParser::OptPrefixContext *ctx) = 0;
  virtual void exitOptPrefix(tigerParser::OptPrefixContext *ctx) = 0;

  virtual void enterExpr(tigerParser::ExprContext *ctx) = 0;
  virtual void exitExpr(tigerParser::ExprContext *ctx) = 0;

  virtual void enterConstRule(tigerParser::ConstRuleContext *ctx) = 0;
  virtual void exitConstRule(tigerParser::ConstRuleContext *ctx) = 0;

  virtual void enterExprList(tigerParser::ExprListContext *ctx) = 0;
  virtual void exitExprList(tigerParser::ExprListContext *ctx) = 0;

  virtual void enterExprListTail(tigerParser::ExprListTailContext *ctx) = 0;
  virtual void exitExprListTail(tigerParser::ExprListTailContext *ctx) = 0;

  virtual void enterLvalue(tigerParser::LvalueContext *ctx) = 0;
  virtual void exitLvalue(tigerParser::LvalueContext *ctx) = 0;

  virtual void enterLvalueTail(tigerParser::LvalueTailContext *ctx) = 0;
  virtual void exitLvalueTail(tigerParser::LvalueTailContext *ctx) = 0;


};

