#include <string>
#include <vector>
#include "UsesExtractor.h"
#include "sp/ast/entity/ProcedureNode.h"

using std::string, std::vector;

UsesExtractor::UsesExtractor(PkbWriter* writer) : pkbWriter(writer),
                                                  oneShot(false) {
}

void UsesExtractor::visitAssign(AssignNode* node) {
  addToPKB.push_back(false);
  isDisabledFromContainer.push_back(false);
  statementStartStack.push_back(node->getLineNumber());
}

void UsesExtractor::visitPrint(PrintNode* node) {
  oneShot = true;
  statementStartStack.push_back(node->getLineNumber());
}

void UsesExtractor::visitWhile(WhileNode* node) {
  addToPKB.push_back(true);
  isDisabledFromContainer.push_back(false);
  statementStartStack.push_back(node->getLineNumber());
}

void UsesExtractor::visitIf(IfNode* node) {
  addToPKB.push_back(true);
  isDisabledFromContainer.push_back(false);
  statementStartStack.push_back(node->getLineNumber());
}

void UsesExtractor::visitStmtList(StatementListNode *node) {
  if (isDisabledFromContainer.empty() || isDisabledFromContainer.back()) {
    return;
  }

  isDisabledFromContainer[isDisabledFromContainer.size() - 1] = true;
  updateUses(readVars);
  readVars.clear();
}

void UsesExtractor::leaveIf(IfNode* node) {
  addToPKB.pop_back();
  isDisabledFromContainer.pop_back();
  statementStartStack.pop_back();
}

void UsesExtractor::leaveWhile(WhileNode* node) {
  addToPKB.pop_back();
  isDisabledFromContainer.pop_back();
  statementStartStack.pop_back();
}

void UsesExtractor::visitProcedure(ProcedureNode* node) {
  procName = node->getName();
}

void UsesExtractor::updateUses(const unordered_set<string> &v) {
  for (int i : statementStartStack) {
    processNode(i, v);
  }
}

void UsesExtractor::updateUses(const string &v) {
  for (int i : statementStartStack) {
    addUsesRelation(i, v);
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

void UsesExtractor::visitVariable(VariableASTNode *node) {
  // Maybe we might need the isDisabledForContainer
  // LHS will encounter this first and set it to true
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

void UsesExtractor::leaveAssign(AssignNode *node) {
  updateUses(readVars);
  addToPKB.pop_back();
  isDisabledFromContainer.pop_back();
  statementStartStack.pop_back();
  readVars.clear();
}
