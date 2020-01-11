//======================== Variable.h ==========================//
// This file defines Variable which is expected to represent an //
// variable in IR. It only implemented a basic framework, and   //
// can be extened as nessissary                                 //
//==============================================================//

#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>

#include "Type.h"

class Value {
 protected:
  Type *Ty;

 public:
  Type *getType();
};

// ConstInt is to represent an integer constant
class ConstInt : public Value {
 public:
  ConstInt(int val);

 private:
  int value;

 public:
  int getValue();
};

// ConstFP is to represent an float constant
class ConstFP : public Value {
 public:
  ConstFP(float val);

 private:
  float value;

 public:
  float getValue();
};

class Variable : public Value {
 public:
  Variable(TypeID TyID, std::string Name);

 private:
  std::string Name;

 public:
  std::string getName();
};

#endif