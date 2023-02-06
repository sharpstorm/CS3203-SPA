#pragma once

#include <vector>
#include "sp/common/AST.h"
#include "pkb/writers/PkbWriter.h"
#include "AbstractExtractor.h"
#include "TreeWalker.h"

using std::vector;

class DesignExtractor {
 public:
  DesignExtractor(PkbWriter* pkbWriter);
  void extract(AST ast);
 private:;
  TreeWalker treeWalker;
  vector<shared_ptr<Extractor>> extractors;
};
