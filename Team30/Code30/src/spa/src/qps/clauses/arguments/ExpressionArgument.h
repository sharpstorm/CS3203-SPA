#pragma once

#include <string>
#include <memory>

using std::string, std::unique_ptr;

class ExpressionArgument {
 public:
  ExpressionArgument(string literalPattern, bool isPartial);
  bool isWildcard();
  bool allowsPartial();
  string getPattern();

 private:
  string literalPattern;
  bool isPartial;
};

typedef unique_ptr<ExpressionArgument> ExpressionArgumentPtr;
