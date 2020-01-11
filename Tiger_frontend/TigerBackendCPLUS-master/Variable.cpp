
#include "Variable.h"

Type *Value::getType() { return Ty; }

ConstInt::ConstInt(int val) : value(val) { Ty = Type::get(INT); }
int ConstInt::getValue() { return value; }

ConstFP::ConstFP(float val) : value(val) { Ty = Type::get(FLOAT); }
float ConstFP::getValue() { return value; }

Variable::Variable(TypeID TyID, std::string Name) : Name(Name) {
  Ty = Type::get(TyID);
}
std::string Variable::getName() { return Name; }
