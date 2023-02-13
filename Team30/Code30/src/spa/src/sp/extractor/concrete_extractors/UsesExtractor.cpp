#include "UsesExtractor.h"

UsesExtractor::UsesExtractor(PkbWriter* writer) { pkbWriter = writer; }

void UsesExtractor::visit(AssignNode node) {
  shared_ptr<ASTNode> expr = node.getChildren()[1];
  processNode(node.lineNumber, expr);
}

void UsesExtractor::visit(PrintNode node) {
  addUsesRelation(node.lineNumber,
                      std::dynamic_pointer_cast<VariableASTNode>
                          (node.getChildren()[0])->getValue());
}

void UsesExtractor::visit(WhileNode node) {
  shared_ptr<ASTNode> condExpr = node.getChildren()[0];
  processNode(node.lineNumber, condExpr);
}

void UsesExtractor::visit(IfNode node) {
  shared_ptr<ASTNode> condExpr = node.getChildren()[0];
  processNode(node.lineNumber, condExpr);
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
    string value = std::dynamic_pointer_cast<VariableASTNode>(node)->getValue();
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
  if(std::find(v->begin(), v->end(), x) != v->end()) {
    return true;
  } else {
    return false;
  }
}
