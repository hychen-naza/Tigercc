
// Generated from tiger.g4 by ANTLR 4.7

#pragma once


#include "antlr4-runtime.h"
#include "tigerVisitor.h"


/**
 * This class provides an empty implementation of tigerVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  tigerBaseVisitor : public tigerVisitor {
public:

  virtual antlrcpp::Any visitProgram(tigerParser::ProgramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDeclarationSegment(tigerParser::DeclarationSegmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTypeDeclarationList(tigerParser::TypeDeclarationListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVarDeclarationList(tigerParser::VarDeclarationListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunctionDeclarationList(tigerParser::FunctionDeclarationListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTypeDeclaration(tigerParser::TypeDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitType(tigerParser::TypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTypeId(tigerParser::TypeIdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVarDeclaration(tigerParser::VarDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIdList(tigerParser::IdListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOptionalInit(tigerParser::OptionalInitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunctionDeclaration(tigerParser::FunctionDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParamList(tigerParser::ParamListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParamListTail(tigerParser::ParamListTailContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRetType(tigerParser::RetTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParam(tigerParser::ParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStatSeq(tigerParser::StatSeqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStat(tigerParser::StatContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitScopeControlFlow(tigerParser::ScopeControlFlowContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStatTail(tigerParser::StatTailContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLtail(tigerParser::LtailContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOptPrefix(tigerParser::OptPrefixContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExpr(tigerParser::ExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConstRule(tigerParser::ConstRuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExprList(tigerParser::ExprListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExprListTail(tigerParser::ExprListTailContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLvalue(tigerParser::LvalueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLvalueTail(tigerParser::LvalueTailContext *ctx) override {
    return visitChildren(ctx);
  }


};

