
#include <assert.h>

#include "BasicBlock.h"
#include "Instruction.h"
#include "Utils.h"

Instruction::Instruction(unsigned opcode, Value *def, std::vector<Value *> ops,
                         BasicBlock *BB)
    : Opcode((enum op)opcode), operands(ops), Parent(BB), defop(def) {
  operands.clear();
  inOps.clear();
  outOps.clear();
}

Value *Instruction::getOperand(int num) {
  assert(num < NumOps);
  return operands[num];
}

unsigned Instruction::getOpcode() { return (unsigned)Opcode; }

std::string Instruction::getOpcodeAsString() {
  std::string str;
  switch (Opcode) {
    case ASSIGN:
      str = "assign";
      break;
    case GOTO:
      str = "goto";
      break;
    case RETURN:
      str = "return";
      break;
    case CALL:
      str = "call";
      break;
    case RCALL:
      str = "rcall";
      break;
    case LOAD:
      str = "load";
      break;
    case STORE:
      str = "store";
      break;
    case ADD:
      str = "add";
      break;
    case SUB:
      str = "sub";
      break;
    case MUL:
      str = "mul";
      break;
    case DIV:
      str = "div";
      break;
    case AND:
      str = "and";
      break;
    case OR:
      str = "or";
      break;
    case BREQ:
      str = "breq";
      break;
    case BRNEQ:
      str = "brneq";
      break;
    case BRLT:
      str = "brlt";
      break;
    case BRGT:
      str = "brgt";
      break;
    case BRGEQ:
      str = "brgeq";
      break;
    case BRLEQ:
      str = "brleq";
      break;
    default:
      str = "invalid";
      break;
  }
  return str;
}

unsigned Instruction::getOpcode(std::string opstr) {
  if (opstr == "assign")
    return ASSIGN;
  else if (opstr == "goto")
    return GOTO;
  else if (opstr == "return")
    return RETURN;
  else if (opstr == "call")
    return CALL;
  else if (opstr == "rcall")
    return RCALL;
  else if (opstr == "load")
    return LOAD;
  else if (opstr == "store")
    return STORE;
  else if (opstr == "add")
    return ADD;
  else if (opstr == "sub")
    return SUB;
  else if (opstr == "mul")
    return MUL;
  else if (opstr == "div")
    return DIV;
  else if (opstr == "and")
    return AND;
  else if (opstr == "or")
    return OR;
  else if (opstr == "breq")
    return BREQ;
  else if (opstr == "brneq")
    return BRNEQ;
  else if (opstr == "brlt")
    return BRLT;
  else if (opstr == "brgt")
    return BRGT;
  else if (opstr == "brgeq")
    return BRGEQ;
  else if (opstr == "brleq")
    return BRLEQ;
  else
    return INVALID;
}

Instruction *Instruction::get(std::string opstr, Value *defop, std::vector<Value *> operands,
                              BasicBlock *BB) {
  unsigned op = getOpcode(opstr);
  if (op == INVALID) {
    std::cerr << "Invalid Instruction opcode: " << opstr << "\n";
    return nullptr;
  }
  return new Instruction(op, defop, operands, BB);
}
