#include <string>
#include <vector>
#include "UsesExtractor.h"
#include "sp/ast/entity/ProcedureNode.h"
#include "ExpressionVariableExtractor.h"

using std::string, std::vector;

UsesExtractor::UsesExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void UsesExtractor::visitAssign(AssignNode* node) {
  ExpressionVariableExtractor variableExtractor;
  node->accept(&variableExtractor);
  updateUses(variableExtractor.getVariableSet(), node->getLineNumber());
}

void UsesExtractor::visitPrint(PrintNode* node) {
  string nodeValue = node->getChildren()[0]->toString();
  addUsesRelation(node->getLineNumber(), nodeValue);
  for (int i : statementStartStack) {
    addUsesRelation(i, nodeValue);
  }
}

void UsesExtractor::visitWhile(WhileNode* node) {
  ExpressionVariableExtractor variableExtractor;
  node->accept(&variableExtractor);
  updateUses(variableExtractor.getVariableSet(), node->getLineNumber());
  statementStartStack.push_back(node->getLineNumber());
}

void UsesExtractor::visitIf(IfNode* node) {
  ExpressionVariableExtractor variableExtractor;
  node->accept(&variableExtractor);
  updateUses(variableExtractor.getVariableSet(), node->getLineNumber());
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

void UsesExtractor::updateUses(unordered_set<string> v,
                               const int &lineNumber) {
  processNode(lineNumber, &v);
  for (int i : statementStartStack) {
    processNode(i, &v);
  }
}

void UsesExtractor::processNode(const int &lineNumber,
                                unordered_set<string>* v) {
  for (const string &s : *v) {
    addUsesRelation(lineNumber, s);
  }
}

void UsesExtractor::addUsesRelation(const int &x, const string &var) {
  pkbWriter->addUses(x, var, procName);
}
