#include <string>
#include <vector>
#include "UsesExtractor.h"
#include "sp/ast/entity/ProcedureNode.h"

using std::string, std::vector;

UsesExtractor::UsesExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void UsesExtractor::visitAssign(AssignNode* node) {
  ASTNode* expr = node->getChildren()[1];
  updateUses(expr, node->getLineNumber());
}

void UsesExtractor::visitPrint(PrintNode* node) {
  string nodeValue = node->getChildren()[0]->toString();
  addUsesRelation(node->getLineNumber(), nodeValue);
  for (int i : statementStartStack) {
    addUsesRelation(i, nodeValue);
  }
}

void UsesExtractor::visitWhile(WhileNode* node) {
  ASTNode* condExpr = node->getChildren()[0];
  updateUses(condExpr, node->getLineNumber());
  statementStartStack.push_back(node->getLineNumber());
}

void UsesExtractor::visitIf(IfNode* node) {
  ASTNode* condExpr = node->getChildren()[0];
  updateUses(condExpr, node->getLineNumber());
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

void UsesExtractor::updateUses(ASTNode* expr,
                               const int &lineNumber) {
  unordered_set<string> v;
  recurseExpr(&v, expr);
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

void UsesExtractor::recurseExpr(unordered_set<string>* v,
                                ASTNode* node) {
  if (node->getType() == ASTNodeType::ASTNODE_CONSTANT) {
    return;
  }

  if (node->getType() == ASTNodeType::ASTNODE_VARIABLE) {
    string value = node->toString();
    if (!setContains(v, value)) {
      v->insert(value);
    }
    return;
  }

  if (node->getChildren()[0] != nullptr) {
    recurseExpr(v, node->getChildren()[0]);
  }
  if (node->getChildren()[1] != nullptr) {
    recurseExpr(v, node->getChildren()[1]);
  }
}

bool UsesExtractor::setContains(unordered_set<string>* v, const string &x) {
  return v->find(x) != v->end();
}

void UsesExtractor::addUsesRelation(const int &x, const string &var) {
  pkbWriter->addUses(x, var, procName);
}
