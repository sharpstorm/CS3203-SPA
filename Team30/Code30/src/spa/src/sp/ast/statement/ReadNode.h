#pragma once

#include <string>
#include "StatementASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::shared_ptr;

class ReadNode : public StatementASTNode {
 public:
  explicit ReadNode(int lineNumber);
  ~ReadNode() = default;

  string toString();
  void accept(Extractor* e);
};
