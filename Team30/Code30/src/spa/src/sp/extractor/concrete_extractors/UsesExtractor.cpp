#include <string>
#include <vector>
#include "UsesExtractor.h"
#include "sp/ast/entity/ProcedureNode.h"

using std::string, std::vector;

UsesExtractor::UsesExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void UsesExtractor::visitAssign(AssignNode* node) {
  shared_ptr<ASTNode> expr = node->getChildren()[1];
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
  shared_ptr<ASTNode> condExpr = node->getChildren()[0];
  updateUses(condExpr, node->getLineNumber());
  statementStartStack.push_back(node->getLineNumber());
}

void UsesExtractor::visitIf(IfNode* node) {
  shared_ptr<ASTNode> condExpr = node->getChildren()[0];
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

void UsesExtractor::updateUses(shared_ptr<ASTNode> expr, int lineNumber) {
  unordered_set<string> v;
  recurseExpr(&v, expr);
  processNode(lineNumber, &v);
  for (int i : statementStartStack) {
    processNode(i, &v);
  }
}

void UsesExtractor::processNode(int lineNumber,
                                unordered_set<string>* v) {
  for (string s : *v) {
    addUsesRelation(lineNumber, s);
  }
}

void UsesExtractor::recurseExpr(unordered_set<string>* v,
                                shared_ptr<ASTNode> node) {
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

void UsesExtractor::addUsesRelation(int x, string var) {
  pkbWriter->addUses(x, var, procName);  // todo
}
