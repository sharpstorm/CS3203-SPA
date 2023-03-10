#include "PatternExtractor.h"

#include <memory>
#include <utility>

#include "common/pattern/PatternConverter.h"
#include "ExpressionVariableExtractor.h"

using std::shared_ptr, std::move;

PatternExtractor::PatternExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void PatternExtractor::visitAssign(AssignNode* node) {
  addPattern(node->getLineNumber(), node->getChild(1));
}

void PatternExtractor::visitWhile(WhileNode* node) {
  ExpressionVariableExtractor variableExtractor;
  node->accept(&variableExtractor);
  for (string s : variableExtractor.getVariableSet()) {
    pkbWriter->addWhilePattern(node->getLineNumber(), s);
  }
}
void PatternExtractor::visitIf(IfNode* node) {
  ExpressionVariableExtractor variableExtractor;
  node->accept(&variableExtractor);
  for (string s : variableExtractor.getVariableSet()) {
    pkbWriter->addIfPattern(node->getLineNumber(), s);
  }
}

void PatternExtractor::addPattern(int x, shared_ptr<IASTNode> node) {
  PatternTriePtr trie = PatternConverter::convertASTToTrie(node);
  PatternTrieSPtr sharedPtr = shared_ptr<PatternTrie>(std::move(trie));
  pkbWriter->addAssigns(x, sharedPtr);
}
