#pragma once

#include <string>
#include <memory>
#include "sp/ast/ASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class ProcedureNode : public ASTNode {
 public:
  explicit ProcedureNode(string name);
  void leave(IExtractor *e) override;
  void accept(IExtractor *e) override;

  string getName();
  ASTNode* getChildStatement(const int &statementIndex);

 private:
  string procName;
};
