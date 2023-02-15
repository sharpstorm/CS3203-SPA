#pragma once

#include <string>

using std::string;

class SemanticErrorStore {
 private:
  string semanticErrorMsg;

 public:
  SemanticErrorStore();
  void setSemanticError(string error);
  bool hasSemanticError();
  string getSemanticError();
};
