#include <memory>
#include <vector>

#include "../../../spa/src/pkb/storage/PKB.h"
#include "../../../spa/src/sp/SourceParser.h"
#include "../../../spa/src/sp/extractor/AbstractExtractor.h"
#include "../../../spa/src/sp/extractor/Extractor.h"
#include "../../../spa/src/sp/extractor/TreeWalker.h"
void executeExtractors(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<shared_ptr<Extractor>> extractors;

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

  extractors.push_back(followsExtractor);
  extractors.push_back(parentExtractor);
  extractors.push_back(entityExtractor);
  extractors.push_back(usesExtractor);
  extractors.push_back(modifiesExtractor);

  AST ast = parser.parseSource(input);
  treeWalker.walkAST(ast, extractors);
}
