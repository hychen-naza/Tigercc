
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>

#include "Module.h"
#include "Utils.h"

Module::Module(const std::string FileName) {
  std::ifstream file(FileName.c_str());
  if (!file.good()) {
    std::cerr << "File " << FileName << " doesn't exist!\n";
    exit(0);
  }

  std::string line;
  std::vector<std::string> buffer;
  buffer.clear();
  while (std::getline(file, line)) {
    auto tmp = trim(line);  // removing heading whitespaces

    // skip comments and empty lines
    if (tmp[0] == '#' || tmp.empty()) continue;

    // remove the whitespaces after ','
    tmp = std::regex_replace(tmp, std::regex(", "), ",");

    // meet a new function, create an Function object for stmts in buffer.
    if (isFunctionDeclStmt(tmp) && !buffer.empty()) {
      Functions.push_back(new Function(buffer));
      buffer.clear();
    }

    buffer.push_back(tmp);
  }

  if (!buffer.empty()) Functions.push_back(new Function(buffer));
  buffer.clear();
}

// isFunctionDeclStmt is to check whether an IR statement
// begines a function definitions.
// FIXME: it currently simply to check whether the statement
// starts with a type and a name (type name), needs to improve
// the regex to support full function type (e.g. type name(args))
const bool Module::isFunctionDeclStmt(std::string stmt) {
  // std::regex fty("(int|void|float)\\s\\w\\((.*)\\):");
  std::regex fty("(int|void|float)\\s\\w");
  std::smatch m;
  return std::regex_search(stmt, m, fty);
}
