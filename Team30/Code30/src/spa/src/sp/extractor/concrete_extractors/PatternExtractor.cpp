#include "PatternExtractor.h"

#include <memory>
#include <utility>
#include <unordered_set>

#include "ExtractorUtility.h"
#include "sp/pattern/TrieBuilder.h"

using std::shared_ptr, std::move;

PatternExtractor::PatternExtractor(PkbWriter *writer) : pkbWriter(writer) {
}

void PatternExtractor::leaveAssign(const AssignNode *node) {
  addPattern(node->getLineNumber(), node->getChild(1));
}

void PatternExtractor::leave(ASTNodeType type, const StatementASTNode *node) {
  ExtractorUtility util;
  VariableNameSet variableSet;
  util.getExprVariables(&variableSet, node->getMutableChild(0));
  for (const VariableName &s : variableSet) {
    if (type == ASTNODE_IF) {
      pkbWriter->addIfPattern(node->getLineNumber(), s);
      continue;
    }
    pkbWriter->addWhilePattern(node->getLineNumber(), s);
  }
}

void PatternExtractor::leaveWhile(const WhileNode *node) {
  leave(ASTNODE_WHILE, node);
}
void PatternExtractor::leaveIf(const IfNode *node) {
  leave(ASTNODE_IF, node);
}

void PatternExtractor::addPattern(LineNumber x, IASTNode *node) {
  PatternTriePtr trie = TrieBuilder(node, pkbWriter).build();
  PatternTrieSPtr sharedPtr = shared_ptr<PatternTrie>(std::move(trie));
  pkbWriter->addAssigns(x, sharedPtr);
}
