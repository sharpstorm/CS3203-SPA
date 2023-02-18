#include <string>
#include "ModifiesExtractor.h"

using std::string;

ModifiesExtractor::ModifiesExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void ModifiesExtractor::visit(AssignNode* node) {
  string nodeValue = node->getChildren()[0]->toString();
  addModifiesRelation(node->getLineNumber(), nodeValue);
  for (int i : statementStartStack) {
    addModifiesRelation(i, nodeValue);
  }
}

void ModifiesExtractor::visit(ReadNode* node) {
  string nodeValue = node->getChildren()[0]->toString();
  addModifiesRelation(node->getLineNumber(), nodeValue);
  for (int i : statementStartStack) {
    addModifiesRelation(i, nodeValue);
  }
}

void ModifiesExtractor::visit(WhileNode* node) {
  statementStartStack.push_back(node->getLineNumber());
}

void ModifiesExtractor::visit(IfNode* node) {
  statementStartStack.push_back(node->getLineNumber());
}

void ModifiesExtractor::leave(IfNode* node) {
  statementStartStack.pop_back();
}

void ModifiesExtractor::leave(WhileNode* node) {
  statementStartStack.pop_back();
}

void ModifiesExtractor::addModifiesRelation(int x, string var) {
  pkbWriter->addModifies(x, var);
}
