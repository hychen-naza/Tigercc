
// Generated from tiger.g4 by ANTLR 4.7

#pragma once


#include "antlr4-runtime.h"
#include "tigerListener.h"


/**
 * This class provides an empty implementation of tigerListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  tigerBaseListener : public tigerListener {
public:

  virtual void enterProgram(tigerParser::ProgramContext * /*ctx*/) override { }
  virtual void exitProgram(tigerParser::ProgramContext * /*ctx*/) override { }

  virtual void enterDeclarationSegment(tigerParser::DeclarationSegmentContext * /*ctx*/) override { }
  virtual void exitDeclarationSegment(tigerParser::DeclarationSegmentContext * /*ctx*/) override { }

  virtual void enterTypeDeclarationList(tigerParser::TypeDeclarationListContext * /*ctx*/) override { }
  virtual void exitTypeDeclarationList(tigerParser::TypeDeclarationListContext * /*ctx*/) override { }

  virtual void enterVarDeclarationList(tigerParser::VarDeclarationListContext * /*ctx*/) override { }
  virtual void exitVarDeclarationList(tigerParser::VarDeclarationListContext * /*ctx*/) override { }

  virtual void enterFunctionDeclarationList(tigerParser::FunctionDeclarationListContext * /*ctx*/) override { }
  virtual void exitFunctionDeclarationList(tigerParser::FunctionDeclarationListContext * /*ctx*/) override { }

  virtual void enterTypeDeclaration(tigerParser::TypeDeclarationContext * /*ctx*/) override { }
  virtual void exitTypeDeclaration(tigerParser::TypeDeclarationContext * /*ctx*/) override { }

  virtual void enterType(tigerParser::TypeContext * /*ctx*/) override { }
  virtual void exitType(tigerParser::TypeContext * /*ctx*/) override { }

  virtual void enterTypeId(tigerParser::TypeIdContext * /*ctx*/) override { }
  virtual void exitTypeId(tigerParser::TypeIdContext * /*ctx*/) override { }

  virtual void enterVarDeclaration(tigerParser::VarDeclarationContext * /*ctx*/) override { }
  virtual void exitVarDeclaration(tigerParser::VarDeclarationContext * /*ctx*/) override { }

  virtual void enterIdList(tigerParser::IdListContext * /*ctx*/) override { }
  virtual void exitIdList(tigerParser::IdListContext * /*ctx*/) override { }

  virtual void enterOptionalInit(tigerParser::OptionalInitContext * /*ctx*/) override { }
  virtual void exitOptionalInit(tigerParser::OptionalInitContext * /*ctx*/) override { }

  virtual void enterFunctionDeclaration(tigerParser::FunctionDeclarationContext * /*ctx*/) override { }
  virtual void exitFunctionDeclaration(tigerParser::FunctionDeclarationContext * /*ctx*/) override { }

  virtual void enterParamList(tigerParser::ParamListContext * /*ctx*/) override { }
  virtual void exitParamList(tigerParser::ParamListContext * /*ctx*/) override { }

  virtual void enterParamListTail(tigerParser::ParamListTailContext * /*ctx*/) override { }
  virtual void exitParamListTail(tigerParser::ParamListTailContext * /*ctx*/) override { }

  virtual void enterRetType(tigerParser::RetTypeContext * /*ctx*/) override { }
  virtual void exitRetType(tigerParser::RetTypeContext * /*ctx*/) override { }

  virtual void enterParam(tigerParser::ParamContext * /*ctx*/) override { }
  virtual void exitParam(tigerParser::ParamContext * /*ctx*/) override { }

  virtual void enterStatSeq(tigerParser::StatSeqContext * /*ctx*/) override { }
  virtual void exitStatSeq(tigerParser::StatSeqContext * /*ctx*/) override { }

  virtual void enterStat(tigerParser::StatContext * /*ctx*/) override { }
  virtual void exitStat(tigerParser::StatContext * /*ctx*/) override { }

  virtual void enterScopeControlFlow(tigerParser::ScopeControlFlowContext * /*ctx*/) override { }
  virtual void exitScopeControlFlow(tigerParser::ScopeControlFlowContext * /*ctx*/) override { }

  virtual void enterStatTail(tigerParser::StatTailContext * /*ctx*/) override { }
  virtual void exitStatTail(tigerParser::StatTailContext * /*ctx*/) override { }

  virtual void enterLtail(tigerParser::LtailContext * /*ctx*/) override { }
  virtual void exitLtail(tigerParser::LtailContext * /*ctx*/) override { }

  virtual void enterOptPrefix(tigerParser::OptPrefixContext * /*ctx*/) override { }
  virtual void exitOptPrefix(tigerParser::OptPrefixContext * /*ctx*/) override { }

  virtual void enterExpr(tigerParser::ExprContext * /*ctx*/) override { }
  virtual void exitExpr(tigerParser::ExprContext * /*ctx*/) override { }

  virtual void enterConstRule(tigerParser::ConstRuleContext * /*ctx*/) override { }
  virtual void exitConstRule(tigerParser::ConstRuleContext * /*ctx*/) override { }

  virtual void enterExprList(tigerParser::ExprListContext * /*ctx*/) override { }
  virtual void exitExprList(tigerParser::ExprListContext * /*ctx*/) override { }

  virtual void enterExprListTail(tigerParser::ExprListTailContext * /*ctx*/) override { }
  virtual void exitExprListTail(tigerParser::ExprListTailContext * /*ctx*/) override { }

  virtual void enterLvalue(tigerParser::LvalueContext * /*ctx*/) override { }
  virtual void exitLvalue(tigerParser::LvalueContext * /*ctx*/) override { }

  virtual void enterLvalueTail(tigerParser::LvalueTailContext * /*ctx*/) override { }
  virtual void exitLvalueTail(tigerParser::LvalueTailContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

