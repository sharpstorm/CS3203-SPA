#include "UsesExtractor.h"
#include "sp/ast/entity/ProcedureNode.h"

UsesExtractor::UsesExtractor(PkbWriter *writer) : pkbWriter(writer),
                                                  oneShot(false) {
}

void UsesExtractor::leave() {
  addToPKB.pop_back();
  isDisabledFromContainer.pop_back();
  statementStartStack.pop_back();
}

void UsesExtractor::visit(bool addToPkb, bool isDisabled,
                          LineNumber lineNumber) {
  addToPKB.push_back(addToPkb);
  isDisabledFromContainer.push_back(isDisabled);
  statementStartStack.push_back(lineNumber);
}

void UsesExtractor::visitAssign(const AssignNode *node) {
  visit(false, false, node->getLineNumber());
}

void UsesExtractor::visitPrint(const PrintNode *node) {
  oneShot = true;
  statementStartStack.push_back(node->getLineNumber());
}

void UsesExtractor::visitWhile(const WhileNode *node) {
  visit(true, false, node->getLineNumber());
}

void UsesExtractor::visitIf(const IfNode *node) {
  visit(true, false, node->getLineNumber());
}

void UsesExtractor::visitStmtList(const StatementListNode *node) {
  if (isDisabledFromContainer.empty() || isDisabledFromContainer.back()) {
    return;
  }

  isDisabledFromContainer[isDisabledFromContainer.size() - 1] = true;
  updateUses(readVars);
  readVars.clear();
}

void UsesExtractor::leaveIf(const IfNode *node) {
  leave();
}

void UsesExtractor::leaveWhile(const WhileNode *node) {
  leave();
}

void UsesExtractor::visitProcedure(const ProcedureNode *node) {
  procName = node->getName();
}

void UsesExtractor::updateUses(const VariableNameSet &v) {
  for (const LineNumber i : statementStartStack) {
    processNode(i, v);
  }
}

void UsesExtractor::updateUses(const VariableName &v) {
  for (const LineNumber i : statementStartStack) {
    addUsesRelation(i, v);
  }
}

void UsesExtractor::processNode(const LineNumber &lineNumber,
                                const VariableNameSet &v) {
  for (const VariableName &s : v) {
    addUsesRelation(lineNumber, s);
  }
}

void UsesExtractor::addUsesRelation(const LineNumber &x,
                                    const VariableName &var) {
  pkbWriter->addUses(x, var, procName);
}

void UsesExtractor::visitVariable(const VariableASTNode *node) {
  if (!addToPKB.empty() && !addToPKB.back()) {
    addToPKB.pop_back();
    addToPKB.push_back(true);
    return;
  }

  if (oneShot) {
    updateUses(node->getValue());
    statementStartStack.pop_back();
    oneShot = false;
    return;
  }

  if (isDisabledFromContainer.empty() || isDisabledFromContainer.back()) {
    return;
  }

  if (readVars.find(node->getValue()) == readVars.end()) {
    readVars.insert(node->getValue());
  }
}

void UsesExtractor::leaveAssign(const AssignNode *node) {
  updateUses(readVars);
  leave();
  readVars.clear();
}
