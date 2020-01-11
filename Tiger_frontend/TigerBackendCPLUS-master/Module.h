#ifndef _MODULE_H_
#define _MODULE_H_

#include <string>
#include <vector>

#include "Function.h"

class Module {
 public:
  Module(std::string FileName);
  ~Module() {
    for (auto it = Functions.begin(); it != Functions.end(); it++) {
      delete *it;
    }
  }

  typedef std::vector<Function *>::iterator iterator;
  iterator begin() { return Functions.begin(); }
  iterator end() { return Functions.end(); }

 private:
  std::vector<Function *> Functions;

 private:
  const bool isFunctionDeclStmt(std::string stmt);
};

#endif