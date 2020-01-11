
#include "Type.h"

Type::Type(TypeID ID) : _ID(ID) {}

Type *Type::get(TypeID ID) { return new Type(ID); }

const bool Type::isCompositeTy() { return _ID == COMPOSITE; }
const bool Type::isIntegerTy() { return _ID == INT; }
const bool Type::isFloatTy() { return _ID == FLOAT; }
const bool Type::isVoidTy() { return _ID == VOID; }

CompositeType::CompositeType(TypeID ETy, int size) : ElemTy(ETy), size(size) {
  _ID = COMPOSITE;
}
TypeID CompositeType::getElemTy() { return ElemTy; }
int CompositeType::getNumElems() { return size; }