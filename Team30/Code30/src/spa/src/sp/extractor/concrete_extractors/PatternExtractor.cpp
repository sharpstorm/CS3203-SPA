#include "PatternExtractor.h"

#include <memory>
#include <utility>
#include <string>
#include <unordered_set>

#include "common/pattern/PatternConverter.h"
#include "ExtractorUtility.h"
#include "sp/errors/SPError.h"

using std::shared_ptr, std::move;

PatternExtractor::PatternExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void PatternExtractor::leave(ASTNodeType type, StatementASTNode *node) {
  ExtractorUtility util;
  unordered_set<string> variableSet;
  util.getExprVariables(&variableSet, node->getChildren()[0]);
  for (string s : variableSet) {
    if (type == ASTNODE_WHILE) {
      pkbWriter->addWhilePattern(node->getLineNumber(), s);
      continue;
    }
    pkbWriter->addIfPattern(node->getLineNumber(), s);
  }
}

void PatternExtractor::leaveAssign(AssignNode* node) {
  addPattern(node->getLineNumber(), node->getChild(1));
}

void PatternExtractor::leaveWhile(WhileNode* node) {
  leave(ASTNODE_WHILE, node);
}
void PatternExtractor::leaveIf(IfNode* node) {
  leave(ASTNODE_IF, node);
}

void PatternExtractor::addPattern(LineNumber x, IASTNode* node) {
  PatternTriePtr trie = PatternConverter::convertASTToTrie(node);
  PatternTrieSPtr sharedPtr = shared_ptr<PatternTrie>(std::move(trie));
  pkbWriter->addAssigns(x, sharedPtr);
}
