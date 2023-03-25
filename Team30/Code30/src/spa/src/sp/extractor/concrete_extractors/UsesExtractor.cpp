#include <string>
#include <vector>
#include "UsesExtractor.h"
#include "sp/ast/entity/ProcedureNode.h"
#include "ExtractorUtility.h"

using std::string, std::vector;

UsesExtractor::UsesExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void UsesExtractor::visitAssign(AssignNode* node) {
  addToPKB.push(false);
  isDisabledFromContainer.push(false);
  statementStartStack.push_back(node->getLineNumber());
  }

  void UsesExtractor::visitPrint(PrintNode* node) {
    string nodeValue = node->getChildren()[0]->toString();
    addUsesRelation(node->getLineNumber(), nodeValue);
    for (int i : statementStartStack) {
      addUsesRelation(i, nodeValue);
    }
  }

  void UsesExtractor::visitWhile(WhileNode* node) {
    addToPKB.push(true);
    isDisabledFromContainer.push(false);
    statementStartStack.push_back(node->getLineNumber());
  }

  void UsesExtractor::visitIf(IfNode* node) {
    addToPKB.push(true);
    isDisabledFromContainer.push(false);
    statementStartStack.push_back(node->getLineNumber());
  }

  void UsesExtractor::leaveIf(IfNode* node) {
    processNode(node->getLineNumber(), readVars);
    addToPKB.pop();
    isDisabledFromContainer.pop();
    statementStartStack.pop_back();
    readVars.clear();
  }

  void UsesExtractor::leaveWhile(WhileNode* node) {
    processNode(node->getLineNumber(), readVars);
    addToPKB.pop();
    isDisabledFromContainer.pop();
    statementStartStack.pop_back();
    readVars.clear();
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

void UsesExtractor::visitVariable(VariableASTNode *node) {
// Maybe we might need the isDisabledForContainer
  // LHS will encounter this first and set it to true
  if (!addToPKB.empty() && !addToPKB.top()) {
    addToPKB.pop();
    addToPKB.push(true);
    return;
  }

  if (readVars.find(node->getValue()) == readVars.end()) {
    readVars.insert(node->getValue());
  }
}

void UsesExtractor::leaveAssign(AssignNode *node) {
  processNode(node->getLineNumber(), readVars);
  addToPKB.pop();
  isDisabledFromContainer.pop();
  statementStartStack.pop_back();
  readVars.clear();
}
