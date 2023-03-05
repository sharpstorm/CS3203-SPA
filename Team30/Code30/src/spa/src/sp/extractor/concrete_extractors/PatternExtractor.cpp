#include "PatternExtractor.h"

#include <memory>
#include <utility>

#include "common/pattern/PatternConverter.h"

using std::shared_ptr, std::move;

PatternExtractor::PatternExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void PatternExtractor::visit(AssignNode* node) {
  addPattern(node->getLineNumber(), node->getChild(1));
}

void PatternExtractor::addPattern(int x, shared_ptr<IASTNode> node) {
  PatternTriePtr trie = PatternConverter::convertASTToTrie(node);
  PatternTrieSPtr sharedPtr = shared_ptr<PatternTrie>(std::move(trie));
  pkbWriter->addAssigns(x, sharedPtr);
}
