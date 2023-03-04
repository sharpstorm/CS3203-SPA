#pragma once

#include <memory>
#include <vector>

#include "AbstractExtractor.h"
#include "TreeWalker.h"
#include "pkb/writers/PkbWriter.h"
#include "sp/ast/AST.h"
#include "sp/extractor/concrete_extractors/FollowsExtractor.h"

using std::vector, std::unique_ptr;

class DesignExtractor {
 public:
  explicit DesignExtractor(PkbWriter* pkbWriter);
  void extract(AST ast);

 private:
  TreeWalker treeWalker;
  vector<unique_ptr<Extractor>> ownedExtractors;
  vector<Extractor*> extractorRefs;
};
