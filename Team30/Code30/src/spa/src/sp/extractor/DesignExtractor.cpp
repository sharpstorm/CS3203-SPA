#include "DesignExtractor.h"
#include "sp/extractor/concrete_extractors/FollowsExtractor.h"
#include "sp/extractor/concrete_extractors/ParentExtractor.h"
#include "sp/extractor/concrete_extractors/EntityExtractor.h"

DesignExtractor::DesignExtractor(PkbWriter* pkbWriter) {
  shared_ptr<AbstractExtractor> followsExtractor =
      shared_ptr<AbstractExtractor>(new FollowsExtractor(pkbWriter));
  shared_ptr<AbstractExtractor> parentExtractor =
      shared_ptr<AbstractExtractor>(new ParentExtractor(pkbWriter));
  shared_ptr<AbstractExtractor> entityExtractor =
      shared_ptr<AbstractExtractor>(new EntityExtractor(pkbWriter));
  extractors.push_back(followsExtractor);
  extractors.push_back(parentExtractor);
  extractors.push_back(entityExtractor);
}

void DesignExtractor::extract(AST ast) { treeWalker.walkAST(ast, extractors); }
