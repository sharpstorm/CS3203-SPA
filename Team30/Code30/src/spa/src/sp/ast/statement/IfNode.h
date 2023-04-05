#pragma once

#include <string>
#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class IfNode : public StatementASTNode {
 public:
  explicit IfNode(LineNumber line);
  ~IfNode() = default;

  void accept(IExtractor* e) override;
  void leave(IExtractor* e) override;
};
