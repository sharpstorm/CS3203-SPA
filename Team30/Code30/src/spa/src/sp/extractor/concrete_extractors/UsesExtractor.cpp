#include <string>
#include <vector>
#include "UsesExtractor.h"

using std::string;

UsesExtractor::UsesExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void UsesExtractor::visit(AssignNode* node) {
  shared_ptr<ASTNode> expr = node->getChildren()[1];
  updateUses(expr, node->getLineNumber());
}

void UsesExtractor::visit(PrintNode* node) {
  string nodeValue = node->getChildren()[0]->toString();
  addUsesRelation(node->getLineNumber(), nodeValue);
  for (int i : statementStartStack) {
    addUsesRelation(i, nodeValue);
  }
}

void UsesExtractor::visit(WhileNode* node) {
  shared_ptr<ASTNode> condExpr = node->getChildren()[0];
  updateUses(condExpr, node->getLineNumber());
  statementStartStack.push_back(node->getLineNumber());
}

void UsesExtractor::visit(IfNode* node) {
  shared_ptr<ASTNode> condExpr = node->getChildren()[0];
  updateUses(condExpr, node->getLineNumber());
  statementStartStack.push_back(node->getLineNumber());
}

void UsesExtractor::leave(IfNode* node) {
  statementStartStack.pop_back();
}

void UsesExtractor::leave(WhileNode* node) {
  statementStartStack.pop_back();
}

void UsesExtractor::updateUses(shared_ptr<ASTNode> expr, int lineNumber) {
  vector<string> v;
  recurseExpr(&v, expr);
  processNode(lineNumber, &v);
  for (int i : statementStartStack) {
    processNode(i, &v);
  }
}

void UsesExtractor::processNode(int lineNumber,
                                vector<string>* v) {
  for (string s : *v) {
    addUsesRelation(lineNumber, s);
  }
}

void UsesExtractor::recurseExpr(vector<string>* v,
                                shared_ptr<ASTNode> node) {
  if (node->getType() == ASTNodeType::ASTNODE_CONSTANT) {
    return;
  }

  if (node->getType() == ASTNodeType::ASTNODE_VARIABLE) {
    string value = node->toString();
    if (!arrayContains(v, value)) {
      v->push_back(value);
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

bool UsesExtractor::arrayContains(vector<string>* v, string x) {
  return std::find(v->begin(), v->end(), x) != v->end();
}

void UsesExtractor::addUsesRelation(int x, string var) {
  pkbWriter->addUses(x, var);
}
