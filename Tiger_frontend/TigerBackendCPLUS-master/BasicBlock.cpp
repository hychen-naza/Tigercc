
#include "BasicBlock.h"

BasicBlock::BasicBlock(std::string Name, Function *F) : Name(Name), Parent(F) {
  Instructions.clear();
}

void BasicBlock::insert(Instruction *Insn) { Instructions.push_back(Insn); }