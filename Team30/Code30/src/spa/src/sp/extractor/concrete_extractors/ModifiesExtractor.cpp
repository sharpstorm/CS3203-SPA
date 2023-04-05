#include "ModifiesExtractor.h"
#include "sp/ast/entity/ProcedureNode.h"

ModifiesExtractor::ModifiesExtractor(PkbWriter* writer) : pkbWriter(writer),
                                                          curStatement(-1) {
}

void ModifiesExtractor::visitAssign(AssignNode* node) {
  curStatement = node->getLineNumber();
}

void ModifiesExtractor::visitRead(ReadNode* node) {
  curStatement = node->getLineNumber();
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

void ModifiesExtractor::visitVariable(VariableASTNode *node) {
  if (curStatement < 0) {
    return;
  }

  addNodeModifies(curStatement, node->getValue());
  curStatement = -1;
}

void ModifiesExtractor::addNodeModifies(const LineNumber &lineNo,
                                        const string &var) {
  addModifiesRelation(lineNo, var);
  for (LineNumber i : statementStartStack) {
    addModifiesRelation(i, var);
  }
}

void ModifiesExtractor::addModifiesRelation(LineNumber x, string var) {
  pkbWriter->addModifies(x, var, currentProcName);
}
