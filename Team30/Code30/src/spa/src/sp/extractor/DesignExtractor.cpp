#include "DesignExtractor.h"

#include <memory>

#include "sp/extractor/concrete_extractors/FollowsExtractor.h"
#include "sp/extractor/concrete_extractors/ParentExtractor.h"
#include "sp/extractor/concrete_extractors/EntityExtractor.h"
#include "sp/extractor/concrete_extractors/UsesExtractor.h"
#include "sp/extractor/concrete_extractors/ModifiesExtractor.h"
#include "sp/extractor/concrete_extractors/PatternExtractor.h"
#include "sp/extractor/concrete_extractors/ProcedureRangeExtractor.h"
#include "sp/extractor/concrete_extractors/CFGExtractor.h"

using std::make_unique;

DesignExtractor::DesignExtractor(PkbWriter* pkbWriter) {
  ownedExtractors.push_back(make_unique<FollowsExtractor>(pkbWriter));
  ownedExtractors.push_back(make_unique<ParentExtractor>(pkbWriter));
  ownedExtractors.push_back(make_unique<EntityExtractor>(pkbWriter));
  ownedExtractors.push_back(make_unique<UsesExtractor>(pkbWriter));
  ownedExtractors.push_back(make_unique<ModifiesExtractor>(pkbWriter));
  ownedExtractors.push_back(make_unique<PatternExtractor>(pkbWriter));
  ownedExtractors.push_back(make_unique<ProcedureRangeExtractor>(pkbWriter));
  ownedExtractors.push_back(make_unique<CFGExtractor>(pkbWriter));

  for (const auto &extractor : ownedExtractors) {
    extractorRefs.push_back(extractor.get());
  }
}

void DesignExtractor::extract(const AST* ast) {
  treeWalker.walkAST(ast, &extractorRefs);
}
