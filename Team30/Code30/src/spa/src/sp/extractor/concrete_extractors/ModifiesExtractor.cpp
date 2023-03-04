#include "ModifiesExtractor.h"
#include "sp/ast/entity/ProcedureNode.h"

ModifiesExtractor::ModifiesExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void ModifiesExtractor::visit(AssignNode* node) {
  string leftVar = node->getChildren()[0]->toString();
  addNodeModifies(node, leftVar);
}

void ModifiesExtractor::visit(ReadNode* node) {
  string var = node->getChildren()[0]->toString();
  addNodeModifies(node, var);
}

void ModifiesExtractor::visit(WhileNode* node) {
  statementStartStack.push_back(node->getLineNumber());
}

void ModifiesExtractor::visit(IfNode* node) {
  statementStartStack.push_back(node->getLineNumber());
}

void ModifiesExtractor::leave(IfNode* node) {
  statementStartStack.pop_back();
}

void ModifiesExtractor::leave(WhileNode* node) {
  statementStartStack.pop_back();
}

void ModifiesExtractor::visit(ProcedureNode* node) {
  currentProcName = node->getName();
}

void ModifiesExtractor::addNodeModifies(StatementASTNode *node,
                                        const string &var) {
  addModifiesRelation(node->getLineNumber(), var);
  for (int i : statementStartStack) {
    addModifiesRelation(i, var);
  }
}

void ModifiesExtractor::addModifiesRelation(int x, string var) {
  pkbWriter->addModifies(x, var, currentProcName);
}
