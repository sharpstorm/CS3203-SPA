#pragma once

#include <string>

#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class WhileNode : public StatementASTNode {
 public:
  explicit WhileNode(LineNumber line);
  ~WhileNode() = default;

  string toString() override;
  void accept(IExtractor* e) override;
  void leave(IExtractor* e) override;
};
