#pragma once

#include <string>
#include <memory>
#include "sp/ast/ASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class ProcedureNode : public ASTNode {
 public:
  explicit ProcedureNode(string name);
  string toString() override;
  void leave(IExtractor *e) override;
  void accept(IExtractor *e) override;

  string getName();

 private:
  string procName;
};
