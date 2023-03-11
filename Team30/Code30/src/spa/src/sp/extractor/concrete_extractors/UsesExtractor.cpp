#include <string>
#include <vector>
#include "UsesExtractor.h"
#include "sp/ast/entity/ProcedureNode.h"
#include "ExtractorUtility.h"

using std::string, std::vector;

UsesExtractor::UsesExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void UsesExtractor::visitAssign(AssignNode* node) {
  ExtractorUtility util;
  unordered_set<string> variableSet;
  util.getExprVariables(&variableSet, node->getChildren()[1]);
  updateUses(variableSet, node->getLineNumber());
}

void UsesExtractor::visitPrint(PrintNode* node) {
  string nodeValue = node->getChildren()[0]->toString();
  addUsesRelation(node->getLineNumber(), nodeValue);
  for (int i : statementStartStack) {
    addUsesRelation(i, nodeValue);
  }
}

void UsesExtractor::visitWhile(WhileNode* node) {
  ExtractorUtility util;
  unordered_set<string> variableSet;
  util.getExprVariables(&variableSet, node->getChildren()[0]);
  updateUses(variableSet, node->getLineNumber());
  statementStartStack.push_back(node->getLineNumber());
}

void UsesExtractor::visitIf(IfNode* node) {
  ExtractorUtility util;
  unordered_set<string> variableSet;
  util.getExprVariables(&variableSet, node->getChildren()[0]);
  updateUses(variableSet, node->getLineNumber());
  statementStartStack.push_back(node->getLineNumber());
}

void UsesExtractor::leaveIf(IfNode* node) {
  statementStartStack.pop_back();
}

void UsesExtractor::leaveWhile(WhileNode* node) {
  statementStartStack.pop_back();
}

void UsesExtractor::visitProcedure(ProcedureNode* node) {
  procName = node->getName();
}

void UsesExtractor::updateUses(const unordered_set<string> &v,
                               const int &lineNumber) {
  processNode(lineNumber, v);
  for (int i : statementStartStack) {
    processNode(i, v);
  }
}

void UsesExtractor::processNode(const int &lineNumber,
                                const unordered_set<string> &v) {
  for (const string &s : v) {
    addUsesRelation(lineNumber, s);
  }
}

void UsesExtractor::addUsesRelation(const int &x, const string &var) {
  pkbWriter->addUses(x, var, procName);
}
