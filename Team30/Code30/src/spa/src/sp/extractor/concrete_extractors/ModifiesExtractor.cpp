#include "ModifiesExtractor.h"
#include "sp/ast/entity/ProcedureNode.h"

ModifiesExtractor::ModifiesExtractor(PkbWriter *writer) :
    pkbWriter(writer), curStatement(NO_LINE) {
}

void ModifiesExtractor::visitAssign(const AssignNode *node) {
  curStatement = node->getLineNumber();
}

void ModifiesExtractor::visitRead(const ReadNode *node) {
  curStatement = node->getLineNumber();
}

void ModifiesExtractor::visitWhile(const WhileNode *node) {
  statementStartStack.push_back(node->getLineNumber());
}

void ModifiesExtractor::visitIf(const IfNode *node) {
  statementStartStack.push_back(node->getLineNumber());
}

void ModifiesExtractor::leaveIf(const IfNode *node) {
  statementStartStack.pop_back();
}

void ModifiesExtractor::leaveWhile(const WhileNode *node) {
  statementStartStack.pop_back();
}

void ModifiesExtractor::visitProcedure(const ProcedureNode *node) {
  currentProcName = node->getName();
}

void ModifiesExtractor::visitVariable(const VariableASTNode *node) {
  if (curStatement < 0) {
    return;
  }

  addNodeModifies(curStatement, node->getValue());
  curStatement = NO_LINE;
}

void ModifiesExtractor::addNodeModifies(const LineNumber &lineNo,
                                        const VariableName &var) {
  addModifiesRelation(lineNo, var);
  for (LineNumber i : statementStartStack) {
    addModifiesRelation(i, var);
  }
}

void ModifiesExtractor::addModifiesRelation(LineNumber x,
                                            const VariableName &var) {
  pkbWriter->addModifies(x, var, currentProcName);
}
