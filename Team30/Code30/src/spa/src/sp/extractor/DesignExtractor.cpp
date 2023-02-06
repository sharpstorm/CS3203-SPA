#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(PkbWriter* pkbWriter) {
  shared_ptr<AbstractExtractor> followsExtractor =
      shared_ptr<AbstractExtractor>(new FollowsExtractor(pkbWriter));
  shared_ptr<AbstractExtractor> parentExtractor =
      shared_ptr<AbstractExtractor>(new ParentExtractor(pkbWriter));
  extractors.push_back(followsExtractor);
  extractors.push_back(parentExtractor);
}

void DesignExtractor::extract(AST ast) { treeWalker.walkAST(ast, extractors); }
