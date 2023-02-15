#include <string>
#include "ModifiesExtractor.h"
#include <iostream>

using std::string;

ModifiesExtractor::ModifiesExtractor(PkbWriter* writer) { pkbWriter = writer; }

void ModifiesExtractor::visit(AssignNode node) {
  string nodeValue = std::dynamic_pointer_cast<VariableASTNode>
      (node.getChildren()[0])->getValue();
  addModifiesRelation(node.lineNumber, nodeValue);
  for (int i : statementStartStack) {
    addModifiesRelation(i, nodeValue);
  }
  checkStack(node.lineNumber);
}

void ModifiesExtractor::visit(ReadNode node) {
  string nodeValue = std::dynamic_pointer_cast<VariableASTNode>
      (node.getChildren()[0])->getValue();
  addModifiesRelation(node.lineNumber, nodeValue);
  for (int i : statementStartStack) {
    addModifiesRelation(i, nodeValue);
  }
  checkStack(node.lineNumber);
}

void ModifiesExtractor::visit(WhileNode node) {
  int end = std::dynamic_pointer_cast<StatementASTNode>
      (node.getChildren()[1]->getChildren().back())->lineNumber;
  statementStartStack.push_back(node.lineNumber);
  statementEndStack.push_back(end);
  checkStack(node.lineNumber);
}

void ModifiesExtractor::visit(IfNode node) {
  int end = std::dynamic_pointer_cast<StatementASTNode>
      (node.getChildren()[2]->getChildren().back())->lineNumber;
  statementStartStack.push_back(node.lineNumber);
  statementEndStack.push_back(end);
  checkStack(node.lineNumber);
}

void ModifiesExtractor::visit(PrintNode node) {
  checkStack(node.lineNumber);
}

void ModifiesExtractor::checkStack(int lineNumber) {
  if (statementStartStack.size() == 0) {
    return;
  }
  if (statementEndStack.back() == lineNumber) {
    statementEndStack.pop_back();
    statementStartStack.pop_back();
  }
}

void ModifiesExtractor::addModifiesRelation(int x, string var) {
  pkbWriter->addModifies(x, var);
}