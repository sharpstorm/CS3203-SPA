#pragma once

#include <vector>

#include "AbstractExtractor.h"
#include "TreeWalker.h"
#include "pkb/writers/PkbWriter.h"
#include "sp/ast/AST.h"
#include "sp/extractor/concrete_extractors/FollowsExtractor.h"

using std::vector;

class DesignExtractor {
 public:
  explicit DesignExtractor(PkbWriter *pkbWriter);
  void extract(const AST *ast);

 private:
  TreeWalker treeWalker;
  vector<IExtractorPtr> ownedExtractors;
  vector<IExtractor *> extractorRefs;
};
