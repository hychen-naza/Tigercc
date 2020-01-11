#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include <string>
#include <vector>

#include "Variable.h"

class BasicBlock;

// Instruction is to represent an IR statment
class Instruction {
 public:
  Instruction(unsigned opcode, Value *defop, std::vector<Value *> ops, BasicBlock *BB);
  ~Instruction();

 private:
  enum op {
    INVALID,
    ASSIGN,
    GOTO,
    RETURN,
    CALL,
    RCALL,
    LOAD,
    STORE,
    ADD,
    SUB,
    MUL,
    DIV,
    AND,
    OR,
    BREQ,
    BRNEQ,
    BRLT,
    BRGT,
    BRGEQ,
    BRLEQ,
  } Opcode;

  BasicBlock *Parent;
  std::vector<Value *> operands;  // all operands involved in the instruction
  Value *defop;
  int NumOps;                     // the number of operands
  std::vector<Value *> inOps;
  std::vector<Value *> outOps;

 public:
  Value *getOperand(int num);
  unsigned getOpcode();
  std::string getOpcodeAsString();

 public:
  static unsigned getOpcode(std::string opstring);
  static Instruction *get(std::string opstr, Value *defop, std::vector<Value *> operands,
                          BasicBlock *BB);
};

#endif