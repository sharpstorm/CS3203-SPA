#include "DesignExtractor.h"
#include "sp/extractor/concrete_extractors/FollowsExtractor.h"
#include "sp/extractor/concrete_extractors/ParentExtractor.h"
#include "sp/extractor/concrete_extractors/EntityExtractor.h"
#include "sp/extractor/concrete_extractors/UsesExtractor.h"
#include "sp/extractor/concrete_extractors/ModifiesExtractor.h"
#include "sp/extractor/concrete_extractors/PatternExtractor.h"

using std::make_unique;

DesignExtractor::DesignExtractor(PkbWriter* pkbWriter) {
  ownedExtractors.push_back(make_unique<FollowsExtractor>(pkbWriter));
  ownedExtractors.push_back(make_unique<ParentExtractor>(pkbWriter));
  ownedExtractors.push_back(make_unique<EntityExtractor>(pkbWriter));
  ownedExtractors.push_back(make_unique<UsesExtractor>(pkbWriter));
  ownedExtractors.push_back(make_unique<ModifiesExtractor>(pkbWriter));
  ownedExtractors.push_back(make_unique<PatternExtractor>(pkbWriter));

  for (int i = 0; i < ownedExtractors.size(); i++) {
    extractorRefs.push_back(ownedExtractors.at(i).get());
  }
}

void DesignExtractor::extract(AST ast) {
  treeWalker.walkAST(ast, &extractorRefs);
}
