#include "PatternExtractor.h"
#include <memory>

PatternExtractor::PatternExtractor(PkbWriter* writer) {
  pkbWriter = writer;
}

void PatternExtractor::visit(AssignNode node) {
  addPattern(node.lineNumber, node.getChild(1));
}

void PatternExtractor::addPattern(int x, shared_ptr<IASTNode> node) {
  pkbWriter->addAssigns(x, node);
}
