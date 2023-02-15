#include <string>
#include <vector>
#include "UsesExtractor.h"

using std::string;

UsesExtractor::UsesExtractor(PkbWriter* writer) { pkbWriter = writer; }

void UsesExtractor::visit(AssignNode node) {
  shared_ptr<ASTNode> expr = node.getChildren()[1];
  processNode(node.lineNumber, expr);
  for (int i : statementStartStack) {
    processNode(i, expr);
  }
}

void UsesExtractor::visit(PrintNode node) {
  string nodeValue = node.getChildren()[0]->toString();
  addUsesRelation(node.lineNumber, nodeValue);
  for (int i : statementStartStack) {
    addUsesRelation(i, nodeValue);
  }
}

void UsesExtractor::visit(WhileNode node) {
  shared_ptr<ASTNode> condExpr = node.getChildren()[0];
  processNode(node.lineNumber, condExpr);
  for (int i : statementStartStack) {
    processNode(i, condExpr);
  }
  statementStartStack.push_back(node.lineNumber);
}

void UsesExtractor::visit(IfNode node) {
  shared_ptr<ASTNode> condExpr = node.getChildren()[0];
  processNode(node.lineNumber, condExpr);
  for (int i : statementStartStack) {
    processNode(i, condExpr);
  }
  statementStartStack.push_back(node.lineNumber);
}

void UsesExtractor::leave(IfNode node) {
  statementStartStack.pop_back();
}

void UsesExtractor::leave(WhileNode node) {
  statementStartStack.pop_back();
}

void UsesExtractor::processNode(int lineNumber,
                                shared_ptr<ASTNode> expr) {
  vector<string> v;
  recurseExpr(&v, expr);
  for (string s : v) {
    addUsesRelation(lineNumber, s);
  }
}

void UsesExtractor::addUsesRelation(int x, string var) {
  pkbWriter->addUses(x, var);
}

void UsesExtractor::recurseExpr(vector<string>* v,
                                shared_ptr<ASTNode> node) {
  if (std::dynamic_pointer_cast<ConstantASTNode>(node) != nullptr) {
    return;
  }
  if (std::dynamic_pointer_cast<VariableASTNode>(node) != nullptr) {
    string value = node->toString();
    if (!arrayContains(v, value)) {
      v->push_back(value);
    }
    return;
  } else {
    if (node->getChildren()[0] != nullptr) {
      recurseExpr(v, node->getChildren()[0]);
    }
    if (node->getChildren()[1] != nullptr) {
      recurseExpr(v, node->getChildren()[1]);
    }
  }
}

bool UsesExtractor::arrayContains(vector<string>* v, string x) {
  return std::find(v->begin(), v->end(), x) != v->end();
}
