#include "ModifiesExtractor.h"
#include "sp/ast/entity/ProcedureNode.h"

ModifiesExtractor::ModifiesExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void ModifiesExtractor::visitAssign(AssignNode* node) {
  string leftVar = node->getChildren()[0]->toString();
  addNodeModifies(node, leftVar);
}

void ModifiesExtractor::visitRead(ReadNode* node) {
  string var = node->getChildren()[0]->toString();
  addNodeModifies(node, var);
}

void ModifiesExtractor::visitWhile(WhileNode* node) {
  statementStartStack.push_back(node->getLineNumber());
}

void ModifiesExtractor::visitIf(IfNode* node) {
  statementStartStack.push_back(node->getLineNumber());
}

void ModifiesExtractor::leaveIf(IfNode* node) {
  statementStartStack.pop_back();
}

void ModifiesExtractor::leaveWhile(WhileNode* node) {
  statementStartStack.pop_back();
}

void ModifiesExtractor::visitProcedure(ProcedureNode* node) {
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
