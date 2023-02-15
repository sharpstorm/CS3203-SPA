#include <string>
#include "ModifiesExtractor.h"

using std::string;

ModifiesExtractor::ModifiesExtractor(PkbWriter* writer) { pkbWriter = writer; }

void ModifiesExtractor::visit(AssignNode node) {
  string nodeValue = std::dynamic_pointer_cast<VariableASTNode>
      (node.getChildren()[0])->getValue();
  addModifiesRelation(node.lineNumber, nodeValue);
  for (int i : statementStartStack) {
    addModifiesRelation(i, nodeValue);
  }
}

void ModifiesExtractor::visit(ReadNode node) {
  string nodeValue = std::dynamic_pointer_cast<VariableASTNode>
      (node.getChildren()[0])->getValue();
  addModifiesRelation(node.lineNumber, nodeValue);
  for (int i : statementStartStack) {
    addModifiesRelation(i, nodeValue);
  }
}

void ModifiesExtractor::visit(WhileNode node) {
  statementStartStack.push_back(node.lineNumber);
}

void ModifiesExtractor::visit(IfNode node) {
  statementStartStack.push_back(node.lineNumber);
}

void ModifiesExtractor::leave(IfNode node) {
  statementStartStack.pop_back();
}

void ModifiesExtractor::leave(WhileNode node) {
  statementStartStack.pop_back();
}

void ModifiesExtractor::addModifiesRelation(int x, string var) {
  pkbWriter->addModifies(x, var);
}