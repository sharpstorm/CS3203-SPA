#include "DesignExtractor.h"
#include "sp/extractor/concrete_extractors/FollowsExtractor.h"
#include "sp/extractor/concrete_extractors/ParentExtractor.h"
#include "sp/extractor/concrete_extractors/EntityExtractor.h"
#include "sp/extractor/concrete_extractors/UsesExtractor.h"
#include "sp/extractor/concrete_extractors/ModifiesExtractor.h"
#include "sp/extractor/concrete_extractors/StatementListExtractor.h"

DesignExtractor::DesignExtractor(PkbWriter* pkbWriter) {
  shared_ptr<AbstractExtractor> followsExtractor =
      shared_ptr<AbstractExtractor>(new FollowsExtractor(pkbWriter));
  shared_ptr<AbstractExtractor> parentExtractor =
      shared_ptr<AbstractExtractor>(new ParentExtractor(pkbWriter));
  shared_ptr<AbstractExtractor> entityExtractor =
      shared_ptr<AbstractExtractor>(new EntityExtractor(pkbWriter));
  shared_ptr<AbstractExtractor> usesExtractor =
      shared_ptr<AbstractExtractor>(new UsesExtractor(pkbWriter));
  shared_ptr<AbstractExtractor> modifiesExtractor =
      shared_ptr<AbstractExtractor>(new ModifiesExtractor(pkbWriter));
  shared_ptr<AbstractExtractor> statementListExtractor =
      shared_ptr<AbstractExtractor>(new StatementListExtractor(pkbWriter));
  extractors.push_back(followsExtractor);
  extractors.push_back(parentExtractor);
  extractors.push_back(entityExtractor);
  extractors.push_back(usesExtractor);
  extractors.push_back(modifiesExtractor);
//  extractors.push_back(statementListExtractor);
}

void DesignExtractor::extract(AST ast) { treeWalker.walkAST(ast, extractors); }
