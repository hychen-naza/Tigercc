#include <iostream>

#include "Function.h"
#include "Utils.h"

Function::Function(std::vector<std::string> stmts) {
  // create an entry basicblock, each function should
  // have at least one basic block (when there is no branches)
  BasicBlock *BB = new BasicBlock("entry", this);
  BasicBlocks.push_back(BB);
  nextStmtofControl = false;
  // stmts[0] is for function prototype
  // and the rest is for statments
  for (int i = 1; i < stmts.size(); i++) {
    auto stmt = stmts[i];

    if (isLabel(stmt)) {
      // create a new basic block
      BB = new BasicBlock(getLabelName(stmt), this);
      BasicBlocks.push_back(BB);
      nextStmtofControl = false;
    } else if (isInstruction(stmt)) {
      // TODO: to add code to create an instruction.
      // you need to do:
      // 1. create an variable/ConstInt/ConstFP object for each operand
      //    it can be retrived from Variables
      // 2. create an Instruction using Instruction::get()
      auto components = split(stmt, ',');

      Value *res = nullptr;
      std::vector<Value *> operands;
      if(components[0].compare("breq")==0 || components[0].compare("brneq")==0 || components[0].compare("brlt")==0 ||\
        components[0].compare("brgt")==0 || components[0].compare("brgeq")==0 || components[0].compare("brleq")==0){
        nextStmtofControl = true;
        operands.push_back(components[1]);
        operands.push_back(components[2]);
      }
      else if(components[0].compare("return")==0){
        operands.push_back(components[1]);
      }
      else if(components[0].compare("assign")==0){
        res = new Variable(, components[1]);
        operands.push_back(components[2]);
      }
      else if(components[0].compare("add")==0 || components[0].compare("sub")==0 || components[0].compare("mul")==0 ||\
        components[0].compare("div")==0 || components[0].compare("and")==0 || components[0].compare("or")==0){
        
      }
      

      /*std::cout << "Stmt: " << stmt << "\n";
      std::cout << "\topstr: " << components[0];      
      for (int i = 1; i < components.size(); i++) {
        std::cout << "\top[" << i << "]: " << components[i];
        operands.push_back();
      }
      std::cout << "\n\n";*/
      
      
      Instruction *inst = Instruction::get(components[0], res, operands, BB);
      BB.insert(inst);

    }
  }
}

const bool Function::isInstruction(std::string stmt) {
  auto components = split(stmt, ',');
  return Instruction::getOpcode(components[0]) != 0;
}

/*const bool Function::isIntegerList(std::string stmt) {
  // TODO: to be implemented
  return false;
}

const bool Function::isFloatList(std::string stmt) {
  // TODO: to be implemented
  return false;
}*/

const bool Function::isLabel(std::string stmt) {
  if(stmt.find("label")==0) return true;
  if(nextStmtofControl) return true;
  return false;
}

std::string Function::getLabelName(std::string LabelStmt) {
  // TODO: to be implemented
  // 暂时就这样
  return LabelStmt;
}
