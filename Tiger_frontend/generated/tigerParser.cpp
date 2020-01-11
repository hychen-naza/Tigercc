
// Generated from tiger.g4 by ANTLR 4.7


#include "tigerListener.h"
#include "tigerVisitor.h"

#include "tigerParser.h"


using namespace antlrcpp;
using namespace antlr4;

tigerParser::tigerParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

tigerParser::~tigerParser() {
  delete _interpreter;
}

std::string tigerParser::getGrammarFileName() const {
  return "tiger.g4";
}

const std::vector<std::string>& tigerParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& tigerParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ProgramContext ------------------------------------------------------------------

tigerParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::ProgramContext::MAIN() {
  return getToken(tigerParser::MAIN, 0);
}

tree::TerminalNode* tigerParser::ProgramContext::LET() {
  return getToken(tigerParser::LET, 0);
}

tigerParser::DeclarationSegmentContext* tigerParser::ProgramContext::declarationSegment() {
  return getRuleContext<tigerParser::DeclarationSegmentContext>(0);
}

tree::TerminalNode* tigerParser::ProgramContext::IN() {
  return getToken(tigerParser::IN, 0);
}

tree::TerminalNode* tigerParser::ProgramContext::BEGIN() {
  return getToken(tigerParser::BEGIN, 0);
}

tigerParser::StatSeqContext* tigerParser::ProgramContext::statSeq() {
  return getRuleContext<tigerParser::StatSeqContext>(0);
}

tree::TerminalNode* tigerParser::ProgramContext::END() {
  return getToken(tigerParser::END, 0);
}


size_t tigerParser::ProgramContext::getRuleIndex() const {
  return tigerParser::RuleProgram;
}

void tigerParser::ProgramContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProgram(this);
}

void tigerParser::ProgramContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProgram(this);
}


antlrcpp::Any tigerParser::ProgramContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitProgram(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::ProgramContext* tigerParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, tigerParser::RuleProgram);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(56);
    match(tigerParser::MAIN);
    setState(57);
    match(tigerParser::LET);
    setState(58);
    declarationSegment();
    setState(59);
    match(tigerParser::IN);
    setState(60);
    match(tigerParser::BEGIN);
    setState(61);
    statSeq();
    setState(62);
    match(tigerParser::END);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationSegmentContext ------------------------------------------------------------------

tigerParser::DeclarationSegmentContext::DeclarationSegmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tigerParser::TypeDeclarationListContext* tigerParser::DeclarationSegmentContext::typeDeclarationList() {
  return getRuleContext<tigerParser::TypeDeclarationListContext>(0);
}

tigerParser::VarDeclarationListContext* tigerParser::DeclarationSegmentContext::varDeclarationList() {
  return getRuleContext<tigerParser::VarDeclarationListContext>(0);
}

tigerParser::FunctionDeclarationListContext* tigerParser::DeclarationSegmentContext::functionDeclarationList() {
  return getRuleContext<tigerParser::FunctionDeclarationListContext>(0);
}


size_t tigerParser::DeclarationSegmentContext::getRuleIndex() const {
  return tigerParser::RuleDeclarationSegment;
}

void tigerParser::DeclarationSegmentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclarationSegment(this);
}

void tigerParser::DeclarationSegmentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclarationSegment(this);
}


antlrcpp::Any tigerParser::DeclarationSegmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitDeclarationSegment(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::DeclarationSegmentContext* tigerParser::declarationSegment() {
  DeclarationSegmentContext *_localctx = _tracker.createInstance<DeclarationSegmentContext>(_ctx, getState());
  enterRule(_localctx, 2, tigerParser::RuleDeclarationSegment);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(64);
    typeDeclarationList();
    setState(65);
    varDeclarationList();
    setState(66);
    functionDeclarationList();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeDeclarationListContext ------------------------------------------------------------------

tigerParser::TypeDeclarationListContext::TypeDeclarationListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tigerParser::TypeDeclarationContext* tigerParser::TypeDeclarationListContext::typeDeclaration() {
  return getRuleContext<tigerParser::TypeDeclarationContext>(0);
}

tigerParser::TypeDeclarationListContext* tigerParser::TypeDeclarationListContext::typeDeclarationList() {
  return getRuleContext<tigerParser::TypeDeclarationListContext>(0);
}


size_t tigerParser::TypeDeclarationListContext::getRuleIndex() const {
  return tigerParser::RuleTypeDeclarationList;
}

void tigerParser::TypeDeclarationListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeDeclarationList(this);
}

void tigerParser::TypeDeclarationListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeDeclarationList(this);
}


antlrcpp::Any tigerParser::TypeDeclarationListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitTypeDeclarationList(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::TypeDeclarationListContext* tigerParser::typeDeclarationList() {
  TypeDeclarationListContext *_localctx = _tracker.createInstance<TypeDeclarationListContext>(_ctx, getState());
  enterRule(_localctx, 4, tigerParser::RuleTypeDeclarationList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(72);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case tigerParser::FUNCTION:
      case tigerParser::IN:
      case tigerParser::VAR: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case tigerParser::TYPE: {
        enterOuterAlt(_localctx, 2);
        setState(69);
        typeDeclaration();
        setState(70);
        typeDeclarationList();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclarationListContext ------------------------------------------------------------------

tigerParser::VarDeclarationListContext::VarDeclarationListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tigerParser::VarDeclarationContext* tigerParser::VarDeclarationListContext::varDeclaration() {
  return getRuleContext<tigerParser::VarDeclarationContext>(0);
}

tigerParser::VarDeclarationListContext* tigerParser::VarDeclarationListContext::varDeclarationList() {
  return getRuleContext<tigerParser::VarDeclarationListContext>(0);
}


size_t tigerParser::VarDeclarationListContext::getRuleIndex() const {
  return tigerParser::RuleVarDeclarationList;
}

void tigerParser::VarDeclarationListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVarDeclarationList(this);
}

void tigerParser::VarDeclarationListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVarDeclarationList(this);
}


antlrcpp::Any tigerParser::VarDeclarationListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitVarDeclarationList(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::VarDeclarationListContext* tigerParser::varDeclarationList() {
  VarDeclarationListContext *_localctx = _tracker.createInstance<VarDeclarationListContext>(_ctx, getState());
  enterRule(_localctx, 6, tigerParser::RuleVarDeclarationList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(78);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case tigerParser::FUNCTION:
      case tigerParser::IN: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case tigerParser::VAR: {
        enterOuterAlt(_localctx, 2);
        setState(75);
        varDeclaration();
        setState(76);
        varDeclarationList();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDeclarationListContext ------------------------------------------------------------------

tigerParser::FunctionDeclarationListContext::FunctionDeclarationListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tigerParser::FunctionDeclarationContext* tigerParser::FunctionDeclarationListContext::functionDeclaration() {
  return getRuleContext<tigerParser::FunctionDeclarationContext>(0);
}

tigerParser::FunctionDeclarationListContext* tigerParser::FunctionDeclarationListContext::functionDeclarationList() {
  return getRuleContext<tigerParser::FunctionDeclarationListContext>(0);
}


size_t tigerParser::FunctionDeclarationListContext::getRuleIndex() const {
  return tigerParser::RuleFunctionDeclarationList;
}

void tigerParser::FunctionDeclarationListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionDeclarationList(this);
}

void tigerParser::FunctionDeclarationListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionDeclarationList(this);
}


antlrcpp::Any tigerParser::FunctionDeclarationListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitFunctionDeclarationList(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::FunctionDeclarationListContext* tigerParser::functionDeclarationList() {
  FunctionDeclarationListContext *_localctx = _tracker.createInstance<FunctionDeclarationListContext>(_ctx, getState());
  enterRule(_localctx, 8, tigerParser::RuleFunctionDeclarationList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(84);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case tigerParser::IN: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case tigerParser::FUNCTION: {
        enterOuterAlt(_localctx, 2);
        setState(81);
        functionDeclaration();
        setState(82);
        functionDeclarationList();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeDeclarationContext ------------------------------------------------------------------

tigerParser::TypeDeclarationContext::TypeDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::TypeDeclarationContext::TYPE() {
  return getToken(tigerParser::TYPE, 0);
}

tree::TerminalNode* tigerParser::TypeDeclarationContext::ID() {
  return getToken(tigerParser::ID, 0);
}

tree::TerminalNode* tigerParser::TypeDeclarationContext::ASSIGN() {
  return getToken(tigerParser::ASSIGN, 0);
}

tigerParser::TypeContext* tigerParser::TypeDeclarationContext::type() {
  return getRuleContext<tigerParser::TypeContext>(0);
}

tree::TerminalNode* tigerParser::TypeDeclarationContext::SEMI() {
  return getToken(tigerParser::SEMI, 0);
}


size_t tigerParser::TypeDeclarationContext::getRuleIndex() const {
  return tigerParser::RuleTypeDeclaration;
}

void tigerParser::TypeDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeDeclaration(this);
}

void tigerParser::TypeDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeDeclaration(this);
}


antlrcpp::Any tigerParser::TypeDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitTypeDeclaration(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::TypeDeclarationContext* tigerParser::typeDeclaration() {
  TypeDeclarationContext *_localctx = _tracker.createInstance<TypeDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 10, tigerParser::RuleTypeDeclaration);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(86);
    match(tigerParser::TYPE);
    setState(87);
    match(tigerParser::ID);
    setState(88);
    match(tigerParser::ASSIGN);
    setState(89);
    type();
    setState(90);
    match(tigerParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

tigerParser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tigerParser::TypeIdContext* tigerParser::TypeContext::typeId() {
  return getRuleContext<tigerParser::TypeIdContext>(0);
}

tree::TerminalNode* tigerParser::TypeContext::ARRAY() {
  return getToken(tigerParser::ARRAY, 0);
}

tree::TerminalNode* tigerParser::TypeContext::LBRACK() {
  return getToken(tigerParser::LBRACK, 0);
}

tree::TerminalNode* tigerParser::TypeContext::INTLIT() {
  return getToken(tigerParser::INTLIT, 0);
}

tree::TerminalNode* tigerParser::TypeContext::RBRACK() {
  return getToken(tigerParser::RBRACK, 0);
}

tree::TerminalNode* tigerParser::TypeContext::OF() {
  return getToken(tigerParser::OF, 0);
}

tree::TerminalNode* tigerParser::TypeContext::ID() {
  return getToken(tigerParser::ID, 0);
}


size_t tigerParser::TypeContext::getRuleIndex() const {
  return tigerParser::RuleType;
}

void tigerParser::TypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterType(this);
}

void tigerParser::TypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitType(this);
}


antlrcpp::Any tigerParser::TypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitType(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::TypeContext* tigerParser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 12, tigerParser::RuleType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(100);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case tigerParser::INT:
      case tigerParser::FLOAT: {
        enterOuterAlt(_localctx, 1);
        setState(92);
        typeId();
        break;
      }

      case tigerParser::ARRAY: {
        enterOuterAlt(_localctx, 2);
        setState(93);
        match(tigerParser::ARRAY);
        setState(94);
        match(tigerParser::LBRACK);
        setState(95);
        match(tigerParser::INTLIT);
        setState(96);
        match(tigerParser::RBRACK);
        setState(97);
        match(tigerParser::OF);
        setState(98);
        typeId();
        break;
      }

      case tigerParser::ID: {
        enterOuterAlt(_localctx, 3);
        setState(99);
        match(tigerParser::ID);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeIdContext ------------------------------------------------------------------

tigerParser::TypeIdContext::TypeIdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::TypeIdContext::INT() {
  return getToken(tigerParser::INT, 0);
}

tree::TerminalNode* tigerParser::TypeIdContext::FLOAT() {
  return getToken(tigerParser::FLOAT, 0);
}


size_t tigerParser::TypeIdContext::getRuleIndex() const {
  return tigerParser::RuleTypeId;
}

void tigerParser::TypeIdContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeId(this);
}

void tigerParser::TypeIdContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeId(this);
}


antlrcpp::Any tigerParser::TypeIdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitTypeId(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::TypeIdContext* tigerParser::typeId() {
  TypeIdContext *_localctx = _tracker.createInstance<TypeIdContext>(_ctx, getState());
  enterRule(_localctx, 14, tigerParser::RuleTypeId);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(102);
    _la = _input->LA(1);
    if (!(_la == tigerParser::INT

    || _la == tigerParser::FLOAT)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclarationContext ------------------------------------------------------------------

tigerParser::VarDeclarationContext::VarDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::VarDeclarationContext::VAR() {
  return getToken(tigerParser::VAR, 0);
}

tigerParser::IdListContext* tigerParser::VarDeclarationContext::idList() {
  return getRuleContext<tigerParser::IdListContext>(0);
}

tree::TerminalNode* tigerParser::VarDeclarationContext::COLON() {
  return getToken(tigerParser::COLON, 0);
}

tigerParser::TypeContext* tigerParser::VarDeclarationContext::type() {
  return getRuleContext<tigerParser::TypeContext>(0);
}

tigerParser::OptionalInitContext* tigerParser::VarDeclarationContext::optionalInit() {
  return getRuleContext<tigerParser::OptionalInitContext>(0);
}

tree::TerminalNode* tigerParser::VarDeclarationContext::SEMI() {
  return getToken(tigerParser::SEMI, 0);
}


size_t tigerParser::VarDeclarationContext::getRuleIndex() const {
  return tigerParser::RuleVarDeclaration;
}

void tigerParser::VarDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVarDeclaration(this);
}

void tigerParser::VarDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVarDeclaration(this);
}


antlrcpp::Any tigerParser::VarDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitVarDeclaration(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::VarDeclarationContext* tigerParser::varDeclaration() {
  VarDeclarationContext *_localctx = _tracker.createInstance<VarDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 16, tigerParser::RuleVarDeclaration);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(104);
    match(tigerParser::VAR);
    setState(105);
    idList();
    setState(106);
    match(tigerParser::COLON);
    setState(107);
    type();
    setState(108);
    optionalInit();
    setState(109);
    match(tigerParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdListContext ------------------------------------------------------------------

tigerParser::IdListContext::IdListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::IdListContext::ID() {
  return getToken(tigerParser::ID, 0);
}

tree::TerminalNode* tigerParser::IdListContext::COMMA() {
  return getToken(tigerParser::COMMA, 0);
}

tigerParser::IdListContext* tigerParser::IdListContext::idList() {
  return getRuleContext<tigerParser::IdListContext>(0);
}


size_t tigerParser::IdListContext::getRuleIndex() const {
  return tigerParser::RuleIdList;
}

void tigerParser::IdListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIdList(this);
}

void tigerParser::IdListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIdList(this);
}


antlrcpp::Any tigerParser::IdListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitIdList(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::IdListContext* tigerParser::idList() {
  IdListContext *_localctx = _tracker.createInstance<IdListContext>(_ctx, getState());
  enterRule(_localctx, 18, tigerParser::RuleIdList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(115);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(111);
      match(tigerParser::ID);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(112);
      match(tigerParser::ID);
      setState(113);
      match(tigerParser::COMMA);
      setState(114);
      idList();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OptionalInitContext ------------------------------------------------------------------

tigerParser::OptionalInitContext::OptionalInitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::OptionalInitContext::ASSIGN() {
  return getToken(tigerParser::ASSIGN, 0);
}

tigerParser::ConstRuleContext* tigerParser::OptionalInitContext::constRule() {
  return getRuleContext<tigerParser::ConstRuleContext>(0);
}


size_t tigerParser::OptionalInitContext::getRuleIndex() const {
  return tigerParser::RuleOptionalInit;
}

void tigerParser::OptionalInitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOptionalInit(this);
}

void tigerParser::OptionalInitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOptionalInit(this);
}


antlrcpp::Any tigerParser::OptionalInitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitOptionalInit(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::OptionalInitContext* tigerParser::optionalInit() {
  OptionalInitContext *_localctx = _tracker.createInstance<OptionalInitContext>(_ctx, getState());
  enterRule(_localctx, 20, tigerParser::RuleOptionalInit);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(120);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case tigerParser::SEMI: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case tigerParser::ASSIGN: {
        enterOuterAlt(_localctx, 2);
        setState(118);
        match(tigerParser::ASSIGN);
        setState(119);
        constRule();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDeclarationContext ------------------------------------------------------------------

tigerParser::FunctionDeclarationContext::FunctionDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::FunctionDeclarationContext::FUNCTION() {
  return getToken(tigerParser::FUNCTION, 0);
}

tree::TerminalNode* tigerParser::FunctionDeclarationContext::ID() {
  return getToken(tigerParser::ID, 0);
}

tree::TerminalNode* tigerParser::FunctionDeclarationContext::LPAREN() {
  return getToken(tigerParser::LPAREN, 0);
}

tigerParser::ParamListContext* tigerParser::FunctionDeclarationContext::paramList() {
  return getRuleContext<tigerParser::ParamListContext>(0);
}

tree::TerminalNode* tigerParser::FunctionDeclarationContext::RPAREN() {
  return getToken(tigerParser::RPAREN, 0);
}

tigerParser::RetTypeContext* tigerParser::FunctionDeclarationContext::retType() {
  return getRuleContext<tigerParser::RetTypeContext>(0);
}

tree::TerminalNode* tigerParser::FunctionDeclarationContext::BEGIN() {
  return getToken(tigerParser::BEGIN, 0);
}

tigerParser::StatSeqContext* tigerParser::FunctionDeclarationContext::statSeq() {
  return getRuleContext<tigerParser::StatSeqContext>(0);
}

tree::TerminalNode* tigerParser::FunctionDeclarationContext::END() {
  return getToken(tigerParser::END, 0);
}

tree::TerminalNode* tigerParser::FunctionDeclarationContext::SEMI() {
  return getToken(tigerParser::SEMI, 0);
}


size_t tigerParser::FunctionDeclarationContext::getRuleIndex() const {
  return tigerParser::RuleFunctionDeclaration;
}

void tigerParser::FunctionDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionDeclaration(this);
}

void tigerParser::FunctionDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionDeclaration(this);
}


antlrcpp::Any tigerParser::FunctionDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitFunctionDeclaration(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::FunctionDeclarationContext* tigerParser::functionDeclaration() {
  FunctionDeclarationContext *_localctx = _tracker.createInstance<FunctionDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 22, tigerParser::RuleFunctionDeclaration);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(122);
    match(tigerParser::FUNCTION);
    setState(123);
    match(tigerParser::ID);
    setState(124);
    match(tigerParser::LPAREN);
    setState(125);
    paramList();
    setState(126);
    match(tigerParser::RPAREN);
    setState(127);
    retType();
    setState(128);
    match(tigerParser::BEGIN);
    setState(129);
    statSeq();
    setState(130);
    match(tigerParser::END);
    setState(131);
    match(tigerParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamListContext ------------------------------------------------------------------

tigerParser::ParamListContext::ParamListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tigerParser::ParamContext* tigerParser::ParamListContext::param() {
  return getRuleContext<tigerParser::ParamContext>(0);
}

tigerParser::ParamListTailContext* tigerParser::ParamListContext::paramListTail() {
  return getRuleContext<tigerParser::ParamListTailContext>(0);
}


size_t tigerParser::ParamListContext::getRuleIndex() const {
  return tigerParser::RuleParamList;
}

void tigerParser::ParamListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParamList(this);
}

void tigerParser::ParamListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParamList(this);
}


antlrcpp::Any tigerParser::ParamListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitParamList(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::ParamListContext* tigerParser::paramList() {
  ParamListContext *_localctx = _tracker.createInstance<ParamListContext>(_ctx, getState());
  enterRule(_localctx, 24, tigerParser::RuleParamList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(137);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case tigerParser::RPAREN: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case tigerParser::ID: {
        enterOuterAlt(_localctx, 2);
        setState(134);
        param();
        setState(135);
        paramListTail();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamListTailContext ------------------------------------------------------------------

tigerParser::ParamListTailContext::ParamListTailContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::ParamListTailContext::COMMA() {
  return getToken(tigerParser::COMMA, 0);
}

tigerParser::ParamContext* tigerParser::ParamListTailContext::param() {
  return getRuleContext<tigerParser::ParamContext>(0);
}

tigerParser::ParamListTailContext* tigerParser::ParamListTailContext::paramListTail() {
  return getRuleContext<tigerParser::ParamListTailContext>(0);
}


size_t tigerParser::ParamListTailContext::getRuleIndex() const {
  return tigerParser::RuleParamListTail;
}

void tigerParser::ParamListTailContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParamListTail(this);
}

void tigerParser::ParamListTailContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParamListTail(this);
}


antlrcpp::Any tigerParser::ParamListTailContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitParamListTail(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::ParamListTailContext* tigerParser::paramListTail() {
  ParamListTailContext *_localctx = _tracker.createInstance<ParamListTailContext>(_ctx, getState());
  enterRule(_localctx, 26, tigerParser::RuleParamListTail);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(144);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case tigerParser::RPAREN: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case tigerParser::COMMA: {
        enterOuterAlt(_localctx, 2);
        setState(140);
        match(tigerParser::COMMA);
        setState(141);
        param();
        setState(142);
        paramListTail();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RetTypeContext ------------------------------------------------------------------

tigerParser::RetTypeContext::RetTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::RetTypeContext::COLON() {
  return getToken(tigerParser::COLON, 0);
}

tigerParser::TypeContext* tigerParser::RetTypeContext::type() {
  return getRuleContext<tigerParser::TypeContext>(0);
}


size_t tigerParser::RetTypeContext::getRuleIndex() const {
  return tigerParser::RuleRetType;
}

void tigerParser::RetTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRetType(this);
}

void tigerParser::RetTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRetType(this);
}


antlrcpp::Any tigerParser::RetTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitRetType(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::RetTypeContext* tigerParser::retType() {
  RetTypeContext *_localctx = _tracker.createInstance<RetTypeContext>(_ctx, getState());
  enterRule(_localctx, 28, tigerParser::RuleRetType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(149);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case tigerParser::BEGIN: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case tigerParser::COLON: {
        enterOuterAlt(_localctx, 2);
        setState(147);
        match(tigerParser::COLON);
        setState(148);
        type();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamContext ------------------------------------------------------------------

tigerParser::ParamContext::ParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::ParamContext::ID() {
  return getToken(tigerParser::ID, 0);
}

tree::TerminalNode* tigerParser::ParamContext::COLON() {
  return getToken(tigerParser::COLON, 0);
}

tigerParser::TypeContext* tigerParser::ParamContext::type() {
  return getRuleContext<tigerParser::TypeContext>(0);
}


size_t tigerParser::ParamContext::getRuleIndex() const {
  return tigerParser::RuleParam;
}

void tigerParser::ParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParam(this);
}

void tigerParser::ParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParam(this);
}


antlrcpp::Any tigerParser::ParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitParam(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::ParamContext* tigerParser::param() {
  ParamContext *_localctx = _tracker.createInstance<ParamContext>(_ctx, getState());
  enterRule(_localctx, 30, tigerParser::RuleParam);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(151);
    match(tigerParser::ID);
    setState(152);
    match(tigerParser::COLON);
    setState(153);
    type();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatSeqContext ------------------------------------------------------------------

tigerParser::StatSeqContext::StatSeqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tigerParser::StatContext* tigerParser::StatSeqContext::stat() {
  return getRuleContext<tigerParser::StatContext>(0);
}

tigerParser::StatSeqContext* tigerParser::StatSeqContext::statSeq() {
  return getRuleContext<tigerParser::StatSeqContext>(0);
}


size_t tigerParser::StatSeqContext::getRuleIndex() const {
  return tigerParser::RuleStatSeq;
}

void tigerParser::StatSeqContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatSeq(this);
}

void tigerParser::StatSeqContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatSeq(this);
}


antlrcpp::Any tigerParser::StatSeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitStatSeq(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::StatSeqContext* tigerParser::statSeq() {
  StatSeqContext *_localctx = _tracker.createInstance<StatSeqContext>(_ctx, getState());
  enterRule(_localctx, 32, tigerParser::RuleStatSeq);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(159);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(155);
      stat();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(156);
      stat();
      setState(157);
      statSeq();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatContext ------------------------------------------------------------------

tigerParser::StatContext::StatContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tigerParser::LvalueContext* tigerParser::StatContext::lvalue() {
  return getRuleContext<tigerParser::LvalueContext>(0);
}

tigerParser::LtailContext* tigerParser::StatContext::ltail() {
  return getRuleContext<tigerParser::LtailContext>(0);
}

tree::TerminalNode* tigerParser::StatContext::ASSIGN() {
  return getToken(tigerParser::ASSIGN, 0);
}

tigerParser::ExprContext* tigerParser::StatContext::expr() {
  return getRuleContext<tigerParser::ExprContext>(0);
}

tree::TerminalNode* tigerParser::StatContext::SEMI() {
  return getToken(tigerParser::SEMI, 0);
}

tigerParser::ScopeControlFlowContext* tigerParser::StatContext::scopeControlFlow() {
  return getRuleContext<tigerParser::ScopeControlFlowContext>(0);
}

tigerParser::OptPrefixContext* tigerParser::StatContext::optPrefix() {
  return getRuleContext<tigerParser::OptPrefixContext>(0);
}

tree::TerminalNode* tigerParser::StatContext::ID() {
  return getToken(tigerParser::ID, 0);
}

tree::TerminalNode* tigerParser::StatContext::LPAREN() {
  return getToken(tigerParser::LPAREN, 0);
}

tigerParser::ExprListContext* tigerParser::StatContext::exprList() {
  return getRuleContext<tigerParser::ExprListContext>(0);
}

tree::TerminalNode* tigerParser::StatContext::RPAREN() {
  return getToken(tigerParser::RPAREN, 0);
}

tree::TerminalNode* tigerParser::StatContext::BREAK() {
  return getToken(tigerParser::BREAK, 0);
}

tree::TerminalNode* tigerParser::StatContext::RETURN() {
  return getToken(tigerParser::RETURN, 0);
}

tree::TerminalNode* tigerParser::StatContext::LET() {
  return getToken(tigerParser::LET, 0);
}

tigerParser::DeclarationSegmentContext* tigerParser::StatContext::declarationSegment() {
  return getRuleContext<tigerParser::DeclarationSegmentContext>(0);
}

tree::TerminalNode* tigerParser::StatContext::IN() {
  return getToken(tigerParser::IN, 0);
}

tigerParser::StatSeqContext* tigerParser::StatContext::statSeq() {
  return getRuleContext<tigerParser::StatSeqContext>(0);
}

tree::TerminalNode* tigerParser::StatContext::END() {
  return getToken(tigerParser::END, 0);
}


size_t tigerParser::StatContext::getRuleIndex() const {
  return tigerParser::RuleStat;
}

void tigerParser::StatContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStat(this);
}

void tigerParser::StatContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStat(this);
}


antlrcpp::Any tigerParser::StatContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitStat(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::StatContext* tigerParser::stat() {
  StatContext *_localctx = _tracker.createInstance<StatContext>(_ctx, getState());
  enterRule(_localctx, 34, tigerParser::RuleStat);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(188);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(161);
      lvalue();
      setState(162);
      ltail();
      setState(163);
      match(tigerParser::ASSIGN);
      setState(164);
      expr(0);
      setState(165);
      match(tigerParser::SEMI);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(167);
      scopeControlFlow();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(168);
      optPrefix();
      setState(169);
      match(tigerParser::ID);
      setState(170);
      match(tigerParser::LPAREN);
      setState(171);
      exprList();
      setState(172);
      match(tigerParser::RPAREN);
      setState(173);
      match(tigerParser::SEMI);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(175);
      match(tigerParser::BREAK);
      setState(176);
      match(tigerParser::SEMI);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(177);
      match(tigerParser::RETURN);
      setState(178);
      expr(0);
      setState(179);
      match(tigerParser::SEMI);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(181);
      match(tigerParser::LET);
      setState(182);
      declarationSegment();
      setState(183);
      match(tigerParser::IN);
      setState(184);
      statSeq();
      setState(185);
      match(tigerParser::END);
      setState(186);
      match(tigerParser::SEMI);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ScopeControlFlowContext ------------------------------------------------------------------

tigerParser::ScopeControlFlowContext::ScopeControlFlowContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::ScopeControlFlowContext::IF() {
  return getToken(tigerParser::IF, 0);
}

std::vector<tigerParser::ExprContext *> tigerParser::ScopeControlFlowContext::expr() {
  return getRuleContexts<tigerParser::ExprContext>();
}

tigerParser::ExprContext* tigerParser::ScopeControlFlowContext::expr(size_t i) {
  return getRuleContext<tigerParser::ExprContext>(i);
}

tree::TerminalNode* tigerParser::ScopeControlFlowContext::THEN() {
  return getToken(tigerParser::THEN, 0);
}

tigerParser::StatSeqContext* tigerParser::ScopeControlFlowContext::statSeq() {
  return getRuleContext<tigerParser::StatSeqContext>(0);
}

tigerParser::StatTailContext* tigerParser::ScopeControlFlowContext::statTail() {
  return getRuleContext<tigerParser::StatTailContext>(0);
}

tree::TerminalNode* tigerParser::ScopeControlFlowContext::WHILE() {
  return getToken(tigerParser::WHILE, 0);
}

tree::TerminalNode* tigerParser::ScopeControlFlowContext::DO() {
  return getToken(tigerParser::DO, 0);
}

tree::TerminalNode* tigerParser::ScopeControlFlowContext::ENDDO() {
  return getToken(tigerParser::ENDDO, 0);
}

tree::TerminalNode* tigerParser::ScopeControlFlowContext::SEMI() {
  return getToken(tigerParser::SEMI, 0);
}

tree::TerminalNode* tigerParser::ScopeControlFlowContext::FOR() {
  return getToken(tigerParser::FOR, 0);
}

tree::TerminalNode* tigerParser::ScopeControlFlowContext::ID() {
  return getToken(tigerParser::ID, 0);
}

tree::TerminalNode* tigerParser::ScopeControlFlowContext::ASSIGN() {
  return getToken(tigerParser::ASSIGN, 0);
}

tree::TerminalNode* tigerParser::ScopeControlFlowContext::TO() {
  return getToken(tigerParser::TO, 0);
}


size_t tigerParser::ScopeControlFlowContext::getRuleIndex() const {
  return tigerParser::RuleScopeControlFlow;
}

void tigerParser::ScopeControlFlowContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterScopeControlFlow(this);
}

void tigerParser::ScopeControlFlowContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitScopeControlFlow(this);
}


antlrcpp::Any tigerParser::ScopeControlFlowContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitScopeControlFlow(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::ScopeControlFlowContext* tigerParser::scopeControlFlow() {
  ScopeControlFlowContext *_localctx = _tracker.createInstance<ScopeControlFlowContext>(_ctx, getState());
  enterRule(_localctx, 36, tigerParser::RuleScopeControlFlow);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(214);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case tigerParser::IF: {
        enterOuterAlt(_localctx, 1);
        setState(190);
        match(tigerParser::IF);
        setState(191);
        expr(0);
        setState(192);
        match(tigerParser::THEN);
        setState(193);
        statSeq();
        setState(194);
        statTail();
        break;
      }

      case tigerParser::WHILE: {
        enterOuterAlt(_localctx, 2);
        setState(196);
        match(tigerParser::WHILE);
        setState(197);
        expr(0);
        setState(198);
        match(tigerParser::DO);
        setState(199);
        statSeq();
        setState(200);
        match(tigerParser::ENDDO);
        setState(201);
        match(tigerParser::SEMI);
        break;
      }

      case tigerParser::FOR: {
        enterOuterAlt(_localctx, 3);
        setState(203);
        match(tigerParser::FOR);
        setState(204);
        match(tigerParser::ID);
        setState(205);
        match(tigerParser::ASSIGN);
        setState(206);
        expr(0);
        setState(207);
        match(tigerParser::TO);
        setState(208);
        expr(0);
        setState(209);
        match(tigerParser::DO);
        setState(210);
        statSeq();
        setState(211);
        match(tigerParser::ENDDO);
        setState(212);
        match(tigerParser::SEMI);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatTailContext ------------------------------------------------------------------

tigerParser::StatTailContext::StatTailContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::StatTailContext::ENDIF() {
  return getToken(tigerParser::ENDIF, 0);
}

tree::TerminalNode* tigerParser::StatTailContext::SEMI() {
  return getToken(tigerParser::SEMI, 0);
}

tree::TerminalNode* tigerParser::StatTailContext::ELSE() {
  return getToken(tigerParser::ELSE, 0);
}

tigerParser::StatSeqContext* tigerParser::StatTailContext::statSeq() {
  return getRuleContext<tigerParser::StatSeqContext>(0);
}


size_t tigerParser::StatTailContext::getRuleIndex() const {
  return tigerParser::RuleStatTail;
}

void tigerParser::StatTailContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatTail(this);
}

void tigerParser::StatTailContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatTail(this);
}


antlrcpp::Any tigerParser::StatTailContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitStatTail(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::StatTailContext* tigerParser::statTail() {
  StatTailContext *_localctx = _tracker.createInstance<StatTailContext>(_ctx, getState());
  enterRule(_localctx, 38, tigerParser::RuleStatTail);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(223);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case tigerParser::ENDIF: {
        enterOuterAlt(_localctx, 1);
        setState(216);
        match(tigerParser::ENDIF);
        setState(217);
        match(tigerParser::SEMI);
        break;
      }

      case tigerParser::ELSE: {
        enterOuterAlt(_localctx, 2);
        setState(218);
        match(tigerParser::ELSE);
        setState(219);
        statSeq();
        setState(220);
        match(tigerParser::ENDIF);
        setState(221);
        match(tigerParser::SEMI);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LtailContext ------------------------------------------------------------------

tigerParser::LtailContext::LtailContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::LtailContext::ASSIGN() {
  return getToken(tigerParser::ASSIGN, 0);
}

tigerParser::LvalueContext* tigerParser::LtailContext::lvalue() {
  return getRuleContext<tigerParser::LvalueContext>(0);
}

tigerParser::LtailContext* tigerParser::LtailContext::ltail() {
  return getRuleContext<tigerParser::LtailContext>(0);
}


size_t tigerParser::LtailContext::getRuleIndex() const {
  return tigerParser::RuleLtail;
}

void tigerParser::LtailContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLtail(this);
}

void tigerParser::LtailContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLtail(this);
}


antlrcpp::Any tigerParser::LtailContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitLtail(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::LtailContext* tigerParser::ltail() {
  LtailContext *_localctx = _tracker.createInstance<LtailContext>(_ctx, getState());
  enterRule(_localctx, 40, tigerParser::RuleLtail);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(230);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(225);
      match(tigerParser::ASSIGN);
      setState(226);
      lvalue();
      setState(227);
      ltail();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);

      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OptPrefixContext ------------------------------------------------------------------

tigerParser::OptPrefixContext::OptPrefixContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tigerParser::LvalueContext* tigerParser::OptPrefixContext::lvalue() {
  return getRuleContext<tigerParser::LvalueContext>(0);
}

tree::TerminalNode* tigerParser::OptPrefixContext::ASSIGN() {
  return getToken(tigerParser::ASSIGN, 0);
}


size_t tigerParser::OptPrefixContext::getRuleIndex() const {
  return tigerParser::RuleOptPrefix;
}

void tigerParser::OptPrefixContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOptPrefix(this);
}

void tigerParser::OptPrefixContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOptPrefix(this);
}


antlrcpp::Any tigerParser::OptPrefixContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitOptPrefix(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::OptPrefixContext* tigerParser::optPrefix() {
  OptPrefixContext *_localctx = _tracker.createInstance<OptPrefixContext>(_ctx, getState());
  enterRule(_localctx, 42, tigerParser::RuleOptPrefix);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(236);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(232);
      lvalue();
      setState(233);
      match(tigerParser::ASSIGN);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);

      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

tigerParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::ExprContext::LPAREN() {
  return getToken(tigerParser::LPAREN, 0);
}

std::vector<tigerParser::ExprContext *> tigerParser::ExprContext::expr() {
  return getRuleContexts<tigerParser::ExprContext>();
}

tigerParser::ExprContext* tigerParser::ExprContext::expr(size_t i) {
  return getRuleContext<tigerParser::ExprContext>(i);
}

tree::TerminalNode* tigerParser::ExprContext::RPAREN() {
  return getToken(tigerParser::RPAREN, 0);
}

tigerParser::ConstRuleContext* tigerParser::ExprContext::constRule() {
  return getRuleContext<tigerParser::ConstRuleContext>(0);
}

tigerParser::LvalueContext* tigerParser::ExprContext::lvalue() {
  return getRuleContext<tigerParser::LvalueContext>(0);
}

tree::TerminalNode* tigerParser::ExprContext::EXP() {
  return getToken(tigerParser::EXP, 0);
}

tree::TerminalNode* tigerParser::ExprContext::MULT() {
  return getToken(tigerParser::MULT, 0);
}

tree::TerminalNode* tigerParser::ExprContext::DIV() {
  return getToken(tigerParser::DIV, 0);
}

tree::TerminalNode* tigerParser::ExprContext::PLUS() {
  return getToken(tigerParser::PLUS, 0);
}

tree::TerminalNode* tigerParser::ExprContext::MINUS() {
  return getToken(tigerParser::MINUS, 0);
}

tree::TerminalNode* tigerParser::ExprContext::EQ() {
  return getToken(tigerParser::EQ, 0);
}

tree::TerminalNode* tigerParser::ExprContext::NEQ() {
  return getToken(tigerParser::NEQ, 0);
}

tree::TerminalNode* tigerParser::ExprContext::GREATER() {
  return getToken(tigerParser::GREATER, 0);
}

tree::TerminalNode* tigerParser::ExprContext::LESSER() {
  return getToken(tigerParser::LESSER, 0);
}

tree::TerminalNode* tigerParser::ExprContext::GREATEREQ() {
  return getToken(tigerParser::GREATEREQ, 0);
}

tree::TerminalNode* tigerParser::ExprContext::LESSEREQ() {
  return getToken(tigerParser::LESSEREQ, 0);
}

tree::TerminalNode* tigerParser::ExprContext::AND() {
  return getToken(tigerParser::AND, 0);
}

tree::TerminalNode* tigerParser::ExprContext::OR() {
  return getToken(tigerParser::OR, 0);
}


size_t tigerParser::ExprContext::getRuleIndex() const {
  return tigerParser::RuleExpr;
}

void tigerParser::ExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpr(this);
}

void tigerParser::ExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpr(this);
}


antlrcpp::Any tigerParser::ExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitExpr(this);
  else
    return visitor->visitChildren(this);
}


tigerParser::ExprContext* tigerParser::expr() {
   return expr(0);
}

tigerParser::ExprContext* tigerParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  tigerParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  tigerParser::ExprContext *previousContext = _localctx;
  size_t startState = 44;
  enterRecursionRule(_localctx, 44, tigerParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(245);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case tigerParser::LPAREN: {
        setState(239);
        match(tigerParser::LPAREN);
        setState(240);
        expr(0);
        setState(241);
        match(tigerParser::RPAREN);
        break;
      }

      case tigerParser::INTLIT:
      case tigerParser::FLOATLIT: {
        setState(243);
        constRule();
        break;
      }

      case tigerParser::ID: {
        setState(244);
        lvalue();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(267);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 17, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(265);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpr);
          setState(247);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(248);
          match(tigerParser::EXP);
          setState(249);
          expr(8);
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpr);
          setState(250);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(251);
          _la = _input->LA(1);
          if (!(_la == tigerParser::MULT

          || _la == tigerParser::DIV)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(252);
          expr(8);
          break;
        }

        case 3: {
          _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpr);
          setState(253);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(254);
          _la = _input->LA(1);
          if (!(_la == tigerParser::PLUS

          || _la == tigerParser::MINUS)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(255);
          expr(7);
          break;
        }

        case 4: {
          _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpr);
          setState(256);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(257);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << tigerParser::EQ)
            | (1ULL << tigerParser::NEQ)
            | (1ULL << tigerParser::LESSER)
            | (1ULL << tigerParser::GREATER)
            | (1ULL << tigerParser::LESSEREQ)
            | (1ULL << tigerParser::GREATEREQ))) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(258);
          expr(6);
          break;
        }

        case 5: {
          _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpr);
          setState(259);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(260);
          match(tigerParser::AND);
          setState(261);
          expr(5);
          break;
        }

        case 6: {
          _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpr);
          setState(262);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(263);
          match(tigerParser::OR);
          setState(264);
          expr(4);
          break;
        }

        } 
      }
      setState(269);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 17, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- ConstRuleContext ------------------------------------------------------------------

tigerParser::ConstRuleContext::ConstRuleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::ConstRuleContext::INTLIT() {
  return getToken(tigerParser::INTLIT, 0);
}

tree::TerminalNode* tigerParser::ConstRuleContext::FLOATLIT() {
  return getToken(tigerParser::FLOATLIT, 0);
}


size_t tigerParser::ConstRuleContext::getRuleIndex() const {
  return tigerParser::RuleConstRule;
}

void tigerParser::ConstRuleContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstRule(this);
}

void tigerParser::ConstRuleContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstRule(this);
}


antlrcpp::Any tigerParser::ConstRuleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitConstRule(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::ConstRuleContext* tigerParser::constRule() {
  ConstRuleContext *_localctx = _tracker.createInstance<ConstRuleContext>(_ctx, getState());
  enterRule(_localctx, 46, tigerParser::RuleConstRule);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(270);
    _la = _input->LA(1);
    if (!(_la == tigerParser::INTLIT

    || _la == tigerParser::FLOATLIT)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprListContext ------------------------------------------------------------------

tigerParser::ExprListContext::ExprListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tigerParser::ExprContext* tigerParser::ExprListContext::expr() {
  return getRuleContext<tigerParser::ExprContext>(0);
}

tigerParser::ExprListTailContext* tigerParser::ExprListContext::exprListTail() {
  return getRuleContext<tigerParser::ExprListTailContext>(0);
}


size_t tigerParser::ExprListContext::getRuleIndex() const {
  return tigerParser::RuleExprList;
}

void tigerParser::ExprListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExprList(this);
}

void tigerParser::ExprListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExprList(this);
}


antlrcpp::Any tigerParser::ExprListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitExprList(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::ExprListContext* tigerParser::exprList() {
  ExprListContext *_localctx = _tracker.createInstance<ExprListContext>(_ctx, getState());
  enterRule(_localctx, 48, tigerParser::RuleExprList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(276);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case tigerParser::RPAREN: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case tigerParser::INTLIT:
      case tigerParser::FLOATLIT:
      case tigerParser::LPAREN:
      case tigerParser::ID: {
        enterOuterAlt(_localctx, 2);
        setState(273);
        expr(0);
        setState(274);
        exprListTail();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprListTailContext ------------------------------------------------------------------

tigerParser::ExprListTailContext::ExprListTailContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::ExprListTailContext::COMMA() {
  return getToken(tigerParser::COMMA, 0);
}

tigerParser::ExprContext* tigerParser::ExprListTailContext::expr() {
  return getRuleContext<tigerParser::ExprContext>(0);
}

tigerParser::ExprListTailContext* tigerParser::ExprListTailContext::exprListTail() {
  return getRuleContext<tigerParser::ExprListTailContext>(0);
}


size_t tigerParser::ExprListTailContext::getRuleIndex() const {
  return tigerParser::RuleExprListTail;
}

void tigerParser::ExprListTailContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExprListTail(this);
}

void tigerParser::ExprListTailContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExprListTail(this);
}


antlrcpp::Any tigerParser::ExprListTailContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitExprListTail(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::ExprListTailContext* tigerParser::exprListTail() {
  ExprListTailContext *_localctx = _tracker.createInstance<ExprListTailContext>(_ctx, getState());
  enterRule(_localctx, 50, tigerParser::RuleExprListTail);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(283);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case tigerParser::COMMA: {
        enterOuterAlt(_localctx, 1);
        setState(278);
        match(tigerParser::COMMA);
        setState(279);
        expr(0);
        setState(280);
        exprListTail();
        break;
      }

      case tigerParser::RPAREN: {
        enterOuterAlt(_localctx, 2);

        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LvalueContext ------------------------------------------------------------------

tigerParser::LvalueContext::LvalueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::LvalueContext::ID() {
  return getToken(tigerParser::ID, 0);
}

tigerParser::LvalueTailContext* tigerParser::LvalueContext::lvalueTail() {
  return getRuleContext<tigerParser::LvalueTailContext>(0);
}


size_t tigerParser::LvalueContext::getRuleIndex() const {
  return tigerParser::RuleLvalue;
}

void tigerParser::LvalueContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLvalue(this);
}

void tigerParser::LvalueContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLvalue(this);
}


antlrcpp::Any tigerParser::LvalueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitLvalue(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::LvalueContext* tigerParser::lvalue() {
  LvalueContext *_localctx = _tracker.createInstance<LvalueContext>(_ctx, getState());
  enterRule(_localctx, 52, tigerParser::RuleLvalue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(285);
    match(tigerParser::ID);
    setState(286);
    lvalueTail();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LvalueTailContext ------------------------------------------------------------------

tigerParser::LvalueTailContext::LvalueTailContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* tigerParser::LvalueTailContext::LBRACK() {
  return getToken(tigerParser::LBRACK, 0);
}

tigerParser::ExprContext* tigerParser::LvalueTailContext::expr() {
  return getRuleContext<tigerParser::ExprContext>(0);
}

tree::TerminalNode* tigerParser::LvalueTailContext::RBRACK() {
  return getToken(tigerParser::RBRACK, 0);
}


size_t tigerParser::LvalueTailContext::getRuleIndex() const {
  return tigerParser::RuleLvalueTail;
}

void tigerParser::LvalueTailContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLvalueTail(this);
}

void tigerParser::LvalueTailContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<tigerListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLvalueTail(this);
}


antlrcpp::Any tigerParser::LvalueTailContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<tigerVisitor*>(visitor))
    return parserVisitor->visitLvalueTail(this);
  else
    return visitor->visitChildren(this);
}

tigerParser::LvalueTailContext* tigerParser::lvalueTail() {
  LvalueTailContext *_localctx = _tracker.createInstance<LvalueTailContext>(_ctx, getState());
  enterRule(_localctx, 54, tigerParser::RuleLvalueTail);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(293);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(288);
      match(tigerParser::LBRACK);
      setState(289);
      expr(0);
      setState(290);
      match(tigerParser::RBRACK);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);

      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool tigerParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 22: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool tigerParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 8);
    case 1: return precpred(_ctx, 7);
    case 2: return precpred(_ctx, 6);
    case 3: return precpred(_ctx, 5);
    case 4: return precpred(_ctx, 4);
    case 5: return precpred(_ctx, 3);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> tigerParser::_decisionToDFA;
atn::PredictionContextCache tigerParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN tigerParser::_atn;
std::vector<uint16_t> tigerParser::_serializedATN;

std::vector<std::string> tigerParser::_ruleNames = {
  "program", "declarationSegment", "typeDeclarationList", "varDeclarationList", 
  "functionDeclarationList", "typeDeclaration", "type", "typeId", "varDeclaration", 
  "idList", "optionalInit", "functionDeclaration", "paramList", "paramListTail", 
  "retType", "param", "statSeq", "stat", "scopeControlFlow", "statTail", 
  "ltail", "optPrefix", "expr", "constRule", "exprList", "exprListTail", 
  "lvalue", "lvalueTail"
};

std::vector<std::string> tigerParser::_literalNames = {
  "", "", "", "", "','", "':'", "';'", "'('", "')'", "'['", "']'", "'{'", 
  "'}'", "'+'", "'-'", "'*'", "'/'", "'**'", "'=='", "'!='", "'<'", "'>'", 
  "'<='", "'>='", "'&'", "'|'", "", "'main'", "'array'", "'break'", "'do'", 
  "'else'", "'for'", "'function'", "'if'", "'in'", "'let'", "'of'", "'then'", 
  "'to'", "'type'", "'var'", "'while'", "'endif'", "'begin'", "'end'", "'enddo'", 
  "'return'", "'int'", "'float'"
};

std::vector<std::string> tigerParser::_symbolicNames = {
  "", "COMMENT", "INTLIT", "FLOATLIT", "COMMA", "COLON", "SEMI", "LPAREN", 
  "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE", "PLUS", "MINUS", "MULT", 
  "DIV", "EXP", "EQ", "NEQ", "LESSER", "GREATER", "LESSEREQ", "GREATEREQ", 
  "AND", "OR", "ASSIGN", "MAIN", "ARRAY", "BREAK", "DO", "ELSE", "FOR", 
  "FUNCTION", "IF", "IN", "LET", "OF", "THEN", "TO", "TYPE", "VAR", "WHILE", 
  "ENDIF", "BEGIN", "END", "ENDDO", "RETURN", "INT", "FLOAT", "ID", "WS"
};

dfa::Vocabulary tigerParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> tigerParser::_tokenNames;

tigerParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x35, 0x12a, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 
    0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15, 
    0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 0x18, 0x9, 
    0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 
    0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 0x9, 0x1d, 0x3, 0x2, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 
    0x4, 0x4b, 0xa, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 
    0x51, 0xa, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x57, 
    0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
    0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 
    0x3, 0x8, 0x5, 0x8, 0x67, 0xa, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 
    0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 0x3, 
    0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x76, 0xa, 0xb, 0x3, 0xc, 0x3, 0xc, 
    0x3, 0xc, 0x5, 0xc, 0x7b, 0xa, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0x8c, 0xa, 0xe, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0x93, 0xa, 
    0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x98, 0xa, 0x10, 0x3, 
    0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x5, 0x12, 0xa2, 0xa, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 
    0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 
    0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0xbf, 0xa, 
    0x13, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 
    0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0xd9, 
    0xa, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 
    0x15, 0x3, 0x15, 0x5, 0x15, 0xe2, 0xa, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 
    0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0xe9, 0xa, 0x16, 0x3, 0x17, 0x3, 
    0x17, 0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0xef, 0xa, 0x17, 0x3, 0x18, 0x3, 
    0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 
    0xf8, 0xa, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 
    0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 
    0x7, 0x18, 0x10c, 0xa, 0x18, 0xc, 0x18, 0xe, 0x18, 0x10f, 0xb, 0x18, 
    0x3, 0x19, 0x3, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x5, 
    0x1a, 0x117, 0xa, 0x1a, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 
    0x3, 0x1b, 0x5, 0x1b, 0x11e, 0xa, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 
    0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x5, 0x1d, 0x128, 
    0xa, 0x1d, 0x3, 0x1d, 0x2, 0x3, 0x2e, 0x1e, 0x2, 0x4, 0x6, 0x8, 0xa, 
    0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 
    0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x2, 
    0x7, 0x3, 0x2, 0x32, 0x33, 0x3, 0x2, 0x11, 0x12, 0x3, 0x2, 0xf, 0x10, 
    0x3, 0x2, 0x14, 0x19, 0x3, 0x2, 0x4, 0x5, 0x2, 0x12d, 0x2, 0x3a, 0x3, 
    0x2, 0x2, 0x2, 0x4, 0x42, 0x3, 0x2, 0x2, 0x2, 0x6, 0x4a, 0x3, 0x2, 0x2, 
    0x2, 0x8, 0x50, 0x3, 0x2, 0x2, 0x2, 0xa, 0x56, 0x3, 0x2, 0x2, 0x2, 0xc, 
    0x58, 0x3, 0x2, 0x2, 0x2, 0xe, 0x66, 0x3, 0x2, 0x2, 0x2, 0x10, 0x68, 
    0x3, 0x2, 0x2, 0x2, 0x12, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x14, 0x75, 0x3, 
    0x2, 0x2, 0x2, 0x16, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x18, 0x7c, 0x3, 0x2, 
    0x2, 0x2, 0x1a, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x92, 0x3, 0x2, 0x2, 
    0x2, 0x1e, 0x97, 0x3, 0x2, 0x2, 0x2, 0x20, 0x99, 0x3, 0x2, 0x2, 0x2, 
    0x22, 0xa1, 0x3, 0x2, 0x2, 0x2, 0x24, 0xbe, 0x3, 0x2, 0x2, 0x2, 0x26, 
    0xd8, 0x3, 0x2, 0x2, 0x2, 0x28, 0xe1, 0x3, 0x2, 0x2, 0x2, 0x2a, 0xe8, 
    0x3, 0x2, 0x2, 0x2, 0x2c, 0xee, 0x3, 0x2, 0x2, 0x2, 0x2e, 0xf7, 0x3, 
    0x2, 0x2, 0x2, 0x30, 0x110, 0x3, 0x2, 0x2, 0x2, 0x32, 0x116, 0x3, 0x2, 
    0x2, 0x2, 0x34, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x36, 0x11f, 0x3, 0x2, 0x2, 
    0x2, 0x38, 0x127, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x3b, 0x7, 0x1d, 0x2, 0x2, 
    0x3b, 0x3c, 0x7, 0x26, 0x2, 0x2, 0x3c, 0x3d, 0x5, 0x4, 0x3, 0x2, 0x3d, 
    0x3e, 0x7, 0x25, 0x2, 0x2, 0x3e, 0x3f, 0x7, 0x2e, 0x2, 0x2, 0x3f, 0x40, 
    0x5, 0x22, 0x12, 0x2, 0x40, 0x41, 0x7, 0x2f, 0x2, 0x2, 0x41, 0x3, 0x3, 
    0x2, 0x2, 0x2, 0x42, 0x43, 0x5, 0x6, 0x4, 0x2, 0x43, 0x44, 0x5, 0x8, 
    0x5, 0x2, 0x44, 0x45, 0x5, 0xa, 0x6, 0x2, 0x45, 0x5, 0x3, 0x2, 0x2, 
    0x2, 0x46, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x47, 0x48, 0x5, 0xc, 0x7, 0x2, 
    0x48, 0x49, 0x5, 0x6, 0x4, 0x2, 0x49, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x4a, 
    0x46, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x47, 0x3, 0x2, 0x2, 0x2, 0x4b, 0x7, 
    0x3, 0x2, 0x2, 0x2, 0x4c, 0x51, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x4e, 0x5, 
    0x12, 0xa, 0x2, 0x4e, 0x4f, 0x5, 0x8, 0x5, 0x2, 0x4f, 0x51, 0x3, 0x2, 
    0x2, 0x2, 0x50, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x50, 0x4d, 0x3, 0x2, 0x2, 
    0x2, 0x51, 0x9, 0x3, 0x2, 0x2, 0x2, 0x52, 0x57, 0x3, 0x2, 0x2, 0x2, 
    0x53, 0x54, 0x5, 0x18, 0xd, 0x2, 0x54, 0x55, 0x5, 0xa, 0x6, 0x2, 0x55, 
    0x57, 0x3, 0x2, 0x2, 0x2, 0x56, 0x52, 0x3, 0x2, 0x2, 0x2, 0x56, 0x53, 
    0x3, 0x2, 0x2, 0x2, 0x57, 0xb, 0x3, 0x2, 0x2, 0x2, 0x58, 0x59, 0x7, 
    0x2a, 0x2, 0x2, 0x59, 0x5a, 0x7, 0x34, 0x2, 0x2, 0x5a, 0x5b, 0x7, 0x1c, 
    0x2, 0x2, 0x5b, 0x5c, 0x5, 0xe, 0x8, 0x2, 0x5c, 0x5d, 0x7, 0x8, 0x2, 
    0x2, 0x5d, 0xd, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x67, 0x5, 0x10, 0x9, 0x2, 
    0x5f, 0x60, 0x7, 0x1e, 0x2, 0x2, 0x60, 0x61, 0x7, 0xb, 0x2, 0x2, 0x61, 
    0x62, 0x7, 0x4, 0x2, 0x2, 0x62, 0x63, 0x7, 0xc, 0x2, 0x2, 0x63, 0x64, 
    0x7, 0x27, 0x2, 0x2, 0x64, 0x67, 0x5, 0x10, 0x9, 0x2, 0x65, 0x67, 0x7, 
    0x34, 0x2, 0x2, 0x66, 0x5e, 0x3, 0x2, 0x2, 0x2, 0x66, 0x5f, 0x3, 0x2, 
    0x2, 0x2, 0x66, 0x65, 0x3, 0x2, 0x2, 0x2, 0x67, 0xf, 0x3, 0x2, 0x2, 
    0x2, 0x68, 0x69, 0x9, 0x2, 0x2, 0x2, 0x69, 0x11, 0x3, 0x2, 0x2, 0x2, 
    0x6a, 0x6b, 0x7, 0x2b, 0x2, 0x2, 0x6b, 0x6c, 0x5, 0x14, 0xb, 0x2, 0x6c, 
    0x6d, 0x7, 0x7, 0x2, 0x2, 0x6d, 0x6e, 0x5, 0xe, 0x8, 0x2, 0x6e, 0x6f, 
    0x5, 0x16, 0xc, 0x2, 0x6f, 0x70, 0x7, 0x8, 0x2, 0x2, 0x70, 0x13, 0x3, 
    0x2, 0x2, 0x2, 0x71, 0x76, 0x7, 0x34, 0x2, 0x2, 0x72, 0x73, 0x7, 0x34, 
    0x2, 0x2, 0x73, 0x74, 0x7, 0x6, 0x2, 0x2, 0x74, 0x76, 0x5, 0x14, 0xb, 
    0x2, 0x75, 0x71, 0x3, 0x2, 0x2, 0x2, 0x75, 0x72, 0x3, 0x2, 0x2, 0x2, 
    0x76, 0x15, 0x3, 0x2, 0x2, 0x2, 0x77, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x78, 
    0x79, 0x7, 0x1c, 0x2, 0x2, 0x79, 0x7b, 0x5, 0x30, 0x19, 0x2, 0x7a, 0x77, 
    0x3, 0x2, 0x2, 0x2, 0x7a, 0x78, 0x3, 0x2, 0x2, 0x2, 0x7b, 0x17, 0x3, 
    0x2, 0x2, 0x2, 0x7c, 0x7d, 0x7, 0x23, 0x2, 0x2, 0x7d, 0x7e, 0x7, 0x34, 
    0x2, 0x2, 0x7e, 0x7f, 0x7, 0x9, 0x2, 0x2, 0x7f, 0x80, 0x5, 0x1a, 0xe, 
    0x2, 0x80, 0x81, 0x7, 0xa, 0x2, 0x2, 0x81, 0x82, 0x5, 0x1e, 0x10, 0x2, 
    0x82, 0x83, 0x7, 0x2e, 0x2, 0x2, 0x83, 0x84, 0x5, 0x22, 0x12, 0x2, 0x84, 
    0x85, 0x7, 0x2f, 0x2, 0x2, 0x85, 0x86, 0x7, 0x8, 0x2, 0x2, 0x86, 0x19, 
    0x3, 0x2, 0x2, 0x2, 0x87, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x88, 0x89, 0x5, 
    0x20, 0x11, 0x2, 0x89, 0x8a, 0x5, 0x1c, 0xf, 0x2, 0x8a, 0x8c, 0x3, 0x2, 
    0x2, 0x2, 0x8b, 0x87, 0x3, 0x2, 0x2, 0x2, 0x8b, 0x88, 0x3, 0x2, 0x2, 
    0x2, 0x8c, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x93, 0x3, 0x2, 0x2, 0x2, 
    0x8e, 0x8f, 0x7, 0x6, 0x2, 0x2, 0x8f, 0x90, 0x5, 0x20, 0x11, 0x2, 0x90, 
    0x91, 0x5, 0x1c, 0xf, 0x2, 0x91, 0x93, 0x3, 0x2, 0x2, 0x2, 0x92, 0x8d, 
    0x3, 0x2, 0x2, 0x2, 0x92, 0x8e, 0x3, 0x2, 0x2, 0x2, 0x93, 0x1d, 0x3, 
    0x2, 0x2, 0x2, 0x94, 0x98, 0x3, 0x2, 0x2, 0x2, 0x95, 0x96, 0x7, 0x7, 
    0x2, 0x2, 0x96, 0x98, 0x5, 0xe, 0x8, 0x2, 0x97, 0x94, 0x3, 0x2, 0x2, 
    0x2, 0x97, 0x95, 0x3, 0x2, 0x2, 0x2, 0x98, 0x1f, 0x3, 0x2, 0x2, 0x2, 
    0x99, 0x9a, 0x7, 0x34, 0x2, 0x2, 0x9a, 0x9b, 0x7, 0x7, 0x2, 0x2, 0x9b, 
    0x9c, 0x5, 0xe, 0x8, 0x2, 0x9c, 0x21, 0x3, 0x2, 0x2, 0x2, 0x9d, 0xa2, 
    0x5, 0x24, 0x13, 0x2, 0x9e, 0x9f, 0x5, 0x24, 0x13, 0x2, 0x9f, 0xa0, 
    0x5, 0x22, 0x12, 0x2, 0xa0, 0xa2, 0x3, 0x2, 0x2, 0x2, 0xa1, 0x9d, 0x3, 
    0x2, 0x2, 0x2, 0xa1, 0x9e, 0x3, 0x2, 0x2, 0x2, 0xa2, 0x23, 0x3, 0x2, 
    0x2, 0x2, 0xa3, 0xa4, 0x5, 0x36, 0x1c, 0x2, 0xa4, 0xa5, 0x5, 0x2a, 0x16, 
    0x2, 0xa5, 0xa6, 0x7, 0x1c, 0x2, 0x2, 0xa6, 0xa7, 0x5, 0x2e, 0x18, 0x2, 
    0xa7, 0xa8, 0x7, 0x8, 0x2, 0x2, 0xa8, 0xbf, 0x3, 0x2, 0x2, 0x2, 0xa9, 
    0xbf, 0x5, 0x26, 0x14, 0x2, 0xaa, 0xab, 0x5, 0x2c, 0x17, 0x2, 0xab, 
    0xac, 0x7, 0x34, 0x2, 0x2, 0xac, 0xad, 0x7, 0x9, 0x2, 0x2, 0xad, 0xae, 
    0x5, 0x32, 0x1a, 0x2, 0xae, 0xaf, 0x7, 0xa, 0x2, 0x2, 0xaf, 0xb0, 0x7, 
    0x8, 0x2, 0x2, 0xb0, 0xbf, 0x3, 0x2, 0x2, 0x2, 0xb1, 0xb2, 0x7, 0x1f, 
    0x2, 0x2, 0xb2, 0xbf, 0x7, 0x8, 0x2, 0x2, 0xb3, 0xb4, 0x7, 0x31, 0x2, 
    0x2, 0xb4, 0xb5, 0x5, 0x2e, 0x18, 0x2, 0xb5, 0xb6, 0x7, 0x8, 0x2, 0x2, 
    0xb6, 0xbf, 0x3, 0x2, 0x2, 0x2, 0xb7, 0xb8, 0x7, 0x26, 0x2, 0x2, 0xb8, 
    0xb9, 0x5, 0x4, 0x3, 0x2, 0xb9, 0xba, 0x7, 0x25, 0x2, 0x2, 0xba, 0xbb, 
    0x5, 0x22, 0x12, 0x2, 0xbb, 0xbc, 0x7, 0x2f, 0x2, 0x2, 0xbc, 0xbd, 0x7, 
    0x8, 0x2, 0x2, 0xbd, 0xbf, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xa3, 0x3, 0x2, 
    0x2, 0x2, 0xbe, 0xa9, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xaa, 0x3, 0x2, 0x2, 
    0x2, 0xbe, 0xb1, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xb3, 0x3, 0x2, 0x2, 0x2, 
    0xbe, 0xb7, 0x3, 0x2, 0x2, 0x2, 0xbf, 0x25, 0x3, 0x2, 0x2, 0x2, 0xc0, 
    0xc1, 0x7, 0x24, 0x2, 0x2, 0xc1, 0xc2, 0x5, 0x2e, 0x18, 0x2, 0xc2, 0xc3, 
    0x7, 0x28, 0x2, 0x2, 0xc3, 0xc4, 0x5, 0x22, 0x12, 0x2, 0xc4, 0xc5, 0x5, 
    0x28, 0x15, 0x2, 0xc5, 0xd9, 0x3, 0x2, 0x2, 0x2, 0xc6, 0xc7, 0x7, 0x2c, 
    0x2, 0x2, 0xc7, 0xc8, 0x5, 0x2e, 0x18, 0x2, 0xc8, 0xc9, 0x7, 0x20, 0x2, 
    0x2, 0xc9, 0xca, 0x5, 0x22, 0x12, 0x2, 0xca, 0xcb, 0x7, 0x30, 0x2, 0x2, 
    0xcb, 0xcc, 0x7, 0x8, 0x2, 0x2, 0xcc, 0xd9, 0x3, 0x2, 0x2, 0x2, 0xcd, 
    0xce, 0x7, 0x22, 0x2, 0x2, 0xce, 0xcf, 0x7, 0x34, 0x2, 0x2, 0xcf, 0xd0, 
    0x7, 0x1c, 0x2, 0x2, 0xd0, 0xd1, 0x5, 0x2e, 0x18, 0x2, 0xd1, 0xd2, 0x7, 
    0x29, 0x2, 0x2, 0xd2, 0xd3, 0x5, 0x2e, 0x18, 0x2, 0xd3, 0xd4, 0x7, 0x20, 
    0x2, 0x2, 0xd4, 0xd5, 0x5, 0x22, 0x12, 0x2, 0xd5, 0xd6, 0x7, 0x30, 0x2, 
    0x2, 0xd6, 0xd7, 0x7, 0x8, 0x2, 0x2, 0xd7, 0xd9, 0x3, 0x2, 0x2, 0x2, 
    0xd8, 0xc0, 0x3, 0x2, 0x2, 0x2, 0xd8, 0xc6, 0x3, 0x2, 0x2, 0x2, 0xd8, 
    0xcd, 0x3, 0x2, 0x2, 0x2, 0xd9, 0x27, 0x3, 0x2, 0x2, 0x2, 0xda, 0xdb, 
    0x7, 0x2d, 0x2, 0x2, 0xdb, 0xe2, 0x7, 0x8, 0x2, 0x2, 0xdc, 0xdd, 0x7, 
    0x21, 0x2, 0x2, 0xdd, 0xde, 0x5, 0x22, 0x12, 0x2, 0xde, 0xdf, 0x7, 0x2d, 
    0x2, 0x2, 0xdf, 0xe0, 0x7, 0x8, 0x2, 0x2, 0xe0, 0xe2, 0x3, 0x2, 0x2, 
    0x2, 0xe1, 0xda, 0x3, 0x2, 0x2, 0x2, 0xe1, 0xdc, 0x3, 0x2, 0x2, 0x2, 
    0xe2, 0x29, 0x3, 0x2, 0x2, 0x2, 0xe3, 0xe4, 0x7, 0x1c, 0x2, 0x2, 0xe4, 
    0xe5, 0x5, 0x36, 0x1c, 0x2, 0xe5, 0xe6, 0x5, 0x2a, 0x16, 0x2, 0xe6, 
    0xe9, 0x3, 0x2, 0x2, 0x2, 0xe7, 0xe9, 0x3, 0x2, 0x2, 0x2, 0xe8, 0xe3, 
    0x3, 0x2, 0x2, 0x2, 0xe8, 0xe7, 0x3, 0x2, 0x2, 0x2, 0xe9, 0x2b, 0x3, 
    0x2, 0x2, 0x2, 0xea, 0xeb, 0x5, 0x36, 0x1c, 0x2, 0xeb, 0xec, 0x7, 0x1c, 
    0x2, 0x2, 0xec, 0xef, 0x3, 0x2, 0x2, 0x2, 0xed, 0xef, 0x3, 0x2, 0x2, 
    0x2, 0xee, 0xea, 0x3, 0x2, 0x2, 0x2, 0xee, 0xed, 0x3, 0x2, 0x2, 0x2, 
    0xef, 0x2d, 0x3, 0x2, 0x2, 0x2, 0xf0, 0xf1, 0x8, 0x18, 0x1, 0x2, 0xf1, 
    0xf2, 0x7, 0x9, 0x2, 0x2, 0xf2, 0xf3, 0x5, 0x2e, 0x18, 0x2, 0xf3, 0xf4, 
    0x7, 0xa, 0x2, 0x2, 0xf4, 0xf8, 0x3, 0x2, 0x2, 0x2, 0xf5, 0xf8, 0x5, 
    0x30, 0x19, 0x2, 0xf6, 0xf8, 0x5, 0x36, 0x1c, 0x2, 0xf7, 0xf0, 0x3, 
    0x2, 0x2, 0x2, 0xf7, 0xf5, 0x3, 0x2, 0x2, 0x2, 0xf7, 0xf6, 0x3, 0x2, 
    0x2, 0x2, 0xf8, 0x10d, 0x3, 0x2, 0x2, 0x2, 0xf9, 0xfa, 0xc, 0xa, 0x2, 
    0x2, 0xfa, 0xfb, 0x7, 0x13, 0x2, 0x2, 0xfb, 0x10c, 0x5, 0x2e, 0x18, 
    0xa, 0xfc, 0xfd, 0xc, 0x9, 0x2, 0x2, 0xfd, 0xfe, 0x9, 0x3, 0x2, 0x2, 
    0xfe, 0x10c, 0x5, 0x2e, 0x18, 0xa, 0xff, 0x100, 0xc, 0x8, 0x2, 0x2, 
    0x100, 0x101, 0x9, 0x4, 0x2, 0x2, 0x101, 0x10c, 0x5, 0x2e, 0x18, 0x9, 
    0x102, 0x103, 0xc, 0x7, 0x2, 0x2, 0x103, 0x104, 0x9, 0x5, 0x2, 0x2, 
    0x104, 0x10c, 0x5, 0x2e, 0x18, 0x8, 0x105, 0x106, 0xc, 0x6, 0x2, 0x2, 
    0x106, 0x107, 0x7, 0x1a, 0x2, 0x2, 0x107, 0x10c, 0x5, 0x2e, 0x18, 0x7, 
    0x108, 0x109, 0xc, 0x5, 0x2, 0x2, 0x109, 0x10a, 0x7, 0x1b, 0x2, 0x2, 
    0x10a, 0x10c, 0x5, 0x2e, 0x18, 0x6, 0x10b, 0xf9, 0x3, 0x2, 0x2, 0x2, 
    0x10b, 0xfc, 0x3, 0x2, 0x2, 0x2, 0x10b, 0xff, 0x3, 0x2, 0x2, 0x2, 0x10b, 
    0x102, 0x3, 0x2, 0x2, 0x2, 0x10b, 0x105, 0x3, 0x2, 0x2, 0x2, 0x10b, 
    0x108, 0x3, 0x2, 0x2, 0x2, 0x10c, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x10d, 
    0x10b, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x10e, 
    0x2f, 0x3, 0x2, 0x2, 0x2, 0x10f, 0x10d, 0x3, 0x2, 0x2, 0x2, 0x110, 0x111, 
    0x9, 0x6, 0x2, 0x2, 0x111, 0x31, 0x3, 0x2, 0x2, 0x2, 0x112, 0x117, 0x3, 
    0x2, 0x2, 0x2, 0x113, 0x114, 0x5, 0x2e, 0x18, 0x2, 0x114, 0x115, 0x5, 
    0x34, 0x1b, 0x2, 0x115, 0x117, 0x3, 0x2, 0x2, 0x2, 0x116, 0x112, 0x3, 
    0x2, 0x2, 0x2, 0x116, 0x113, 0x3, 0x2, 0x2, 0x2, 0x117, 0x33, 0x3, 0x2, 
    0x2, 0x2, 0x118, 0x119, 0x7, 0x6, 0x2, 0x2, 0x119, 0x11a, 0x5, 0x2e, 
    0x18, 0x2, 0x11a, 0x11b, 0x5, 0x34, 0x1b, 0x2, 0x11b, 0x11e, 0x3, 0x2, 
    0x2, 0x2, 0x11c, 0x11e, 0x3, 0x2, 0x2, 0x2, 0x11d, 0x118, 0x3, 0x2, 
    0x2, 0x2, 0x11d, 0x11c, 0x3, 0x2, 0x2, 0x2, 0x11e, 0x35, 0x3, 0x2, 0x2, 
    0x2, 0x11f, 0x120, 0x7, 0x34, 0x2, 0x2, 0x120, 0x121, 0x5, 0x38, 0x1d, 
    0x2, 0x121, 0x37, 0x3, 0x2, 0x2, 0x2, 0x122, 0x123, 0x7, 0xb, 0x2, 0x2, 
    0x123, 0x124, 0x5, 0x2e, 0x18, 0x2, 0x124, 0x125, 0x7, 0xc, 0x2, 0x2, 
    0x125, 0x128, 0x3, 0x2, 0x2, 0x2, 0x126, 0x128, 0x3, 0x2, 0x2, 0x2, 
    0x127, 0x122, 0x3, 0x2, 0x2, 0x2, 0x127, 0x126, 0x3, 0x2, 0x2, 0x2, 
    0x128, 0x39, 0x3, 0x2, 0x2, 0x2, 0x17, 0x4a, 0x50, 0x56, 0x66, 0x75, 
    0x7a, 0x8b, 0x92, 0x97, 0xa1, 0xbe, 0xd8, 0xe1, 0xe8, 0xee, 0xf7, 0x10b, 
    0x10d, 0x116, 0x11d, 0x127, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

tigerParser::Initializer tigerParser::_init;
