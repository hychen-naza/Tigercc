#ifndef _BASICBLOCK_H_
#define _BASICBLOCK_H_

#include <vector>

#include "Instruction.h"

class Function;

class BasicBlock {
 public:
  BasicBlock(std::string Name, Function *F);

 private:
  Function *Parent;
  std::string Name;
  std::vector<Instruction *> Instructions;

 public:
  typedef std::vector<Instruction *>::iterator iterator;
  iterator begin() { return Instructions.begin(); }
  iterator end() { return Instructions.end(); }
  void insert(Instruction *Insn);
};

#endif