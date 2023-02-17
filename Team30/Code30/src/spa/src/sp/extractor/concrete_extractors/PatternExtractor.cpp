#include "PatternExtractor.h"

PatternExtractor::PatternExtractor(PkbWriter* writer) {
  pkbWriter = writer;
}

void PatternExtractor::visit(AssignNode node) {
  // expr: node.getChildren()[1];
  //writer->addAssigns(node->lineNumber, node
}