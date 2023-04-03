#pragma once

#include <string>
#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class AssignNode : public StatementASTNode {
 public:
  explicit AssignNode(LineNumber line);
  ~AssignNode() = default;

  void accept(IExtractor* e) override;
  void leave(IExtractor* e) override;
};
