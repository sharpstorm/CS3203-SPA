#pragma once

#include <string>

using std::string;

class SemanticErrorThrower {
 private:
  string semanticErrorMsg;

 public:
  SemanticErrorThrower();
  void setSemanticError(string error);
  bool hasSemanticError();
  void throwIfSemanticError();
};
