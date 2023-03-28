#pragma once

#include <string>
#include "StatementASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class ReadNode : public StatementASTNode {
 public:
  explicit ReadNode(LineNumber line);
  ~ReadNode() = default;

  string toString() override;
  void accept(IExtractor* e) override;
};
