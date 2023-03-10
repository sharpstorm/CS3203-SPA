#include "PatternExtractor.h"

#include <memory>
#include <utility>
#include <string>
#include <unordered_set>

#include "common/pattern/PatternConverter.h"
#include "ExtractorUtility.h"

using std::shared_ptr, std::move;

PatternExtractor::PatternExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void PatternExtractor::visitAssign(AssignNode* node) {
  addPattern(node->getLineNumber(), node->getChild(1));
}

void PatternExtractor::visitWhile(WhileNode* node) {
  ExtractorUtility util;
  unordered_set<string> variableSet;
  util.getExprVariables(&variableSet, node->getChildren()[0]);
  for (string s : variableSet) {
    pkbWriter->addWhilePattern(node->getLineNumber(), s);
  }
}
void PatternExtractor::visitIf(IfNode* node) {
  ExtractorUtility util;
  unordered_set<string> variableSet;
  util.getExprVariables(&variableSet, node->getChildren()[0]);
  for (string s : variableSet) {
    pkbWriter->addIfPattern(node->getLineNumber(), s);
  }
}

void PatternExtractor::addPattern(int x, IASTNode* node) {
  PatternTriePtr trie = PatternConverter::convertASTToTrie(node);
  PatternTrieSPtr sharedPtr = shared_ptr<PatternTrie>(std::move(trie));
  pkbWriter->addAssigns(x, sharedPtr);
}
