#ifndef _TYPE_H_
#define _TYPE_H_

typedef enum _T { VOID, INT, FLOAT, COMPOSITE } TypeID;

class Type {
 public:
  Type(TypeID ID);

 protected:
  TypeID _ID;

 public:
  const bool isCompositeTy();
  const bool isIntegerTy();
  const bool isFloatTy();
  const bool isVoidTy();

  static Type *get(TypeID TyID);
};

// the CompositeType is used to represent an array
class CompositeType : public Type {
 public:
  CompositeType(TypeID elemTy, int size);

 private:
  TypeID ElemTy;
  int size;

 public:
  TypeID getElemTy();
  int getNumElems();
};

#endif