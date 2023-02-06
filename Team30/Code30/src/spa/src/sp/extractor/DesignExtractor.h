#pragma once

#include <memory>
#include <vector>

#include "AbstractExtractor.h"
#include "TreeWalker.h"
#include "pkb/writers/PkbWriter.h"
#include "sp/common/AST.h"

using std::vector;

class DesignExtractor {
 public:
  explicit DesignExtractor(PkbWriter* pkbWriter);
  void extract(AST ast);

 private:
  TreeWalker treeWalker;
  vector<shared_ptr<Extractor>> extractors;
};
