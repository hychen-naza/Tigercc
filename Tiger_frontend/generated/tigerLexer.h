
// Generated from tiger.g4 by ANTLR 4.7

#pragma once


#include "antlr4-runtime.h"




class  tigerLexer : public antlr4::Lexer {
public:
  enum {
    COMMENT = 1, INTLIT = 2, FLOATLIT = 3, COMMA = 4, COLON = 5, SEMI = 6, 
    LPAREN = 7, RPAREN = 8, LBRACK = 9, RBRACK = 10, LBRACE = 11, RBRACE = 12, 
    PLUS = 13, MINUS = 14, MULT = 15, DIV = 16, EXP = 17, EQ = 18, NEQ = 19, 
    LESSER = 20, GREATER = 21, LESSEREQ = 22, GREATEREQ = 23, AND = 24, 
    OR = 25, ASSIGN = 26, MAIN = 27, ARRAY = 28, BREAK = 29, DO = 30, ELSE = 31, 
    FOR = 32, FUNCTION = 33, IF = 34, IN = 35, LET = 36, OF = 37, THEN = 38, 
    TO = 39, TYPE = 40, VAR = 41, WHILE = 42, ENDIF = 43, BEGIN = 44, END = 45, 
    ENDDO = 46, RETURN = 47, INT = 48, FLOAT = 49, ID = 50, WS = 51
  };

  tigerLexer(antlr4::CharStream *input);
  ~tigerLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

