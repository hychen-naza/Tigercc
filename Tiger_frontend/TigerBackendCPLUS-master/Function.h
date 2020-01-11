#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <map>
#include <vector>

#include "BasicBlock.h"
#include "Variable.h"

class Function {
 public:
  Function(std::vector<std::string> stmts);
  ~Function() {
    for (auto it = BasicBlocks.begin(); it != BasicBlocks.end(); it++) {
      delete *it;
    }
    for (auto x : Variables) {
      delete x.second;
    }
  }

 private:
  std::map<std::string, Value *> Variables;
  std::vector<BasicBlock *> BasicBlocks;
  std::vector<Value *> args;
  Type *ReTy;
  bool nextStmtofControl;

 private:
  const bool isInstruction(std::string stmt);
  const bool isIntegerList(std::string stmt);
  const bool isFloatList(std::string stmt);
  const bool isLabel(std::string stmt);
  std::string getLabelName(std::string LabelStmt);
};

#endif