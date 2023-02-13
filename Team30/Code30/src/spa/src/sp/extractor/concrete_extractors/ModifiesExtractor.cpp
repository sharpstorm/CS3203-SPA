#include <string>
#include "ModifiesExtractor.h"

using std::string;

ModifiesExtractor::ModifiesExtractor(PkbWriter* writer) { pkbWriter = writer; }

void ModifiesExtractor::visit(AssignNode node) {
  addModifiesRelation(node.lineNumber,
                      std::dynamic_pointer_cast<VariableASTNode>
                          (node.getChildren()[0])->getValue());
}

void ModifiesExtractor::visit(ReadNode node) {
  addModifiesRelation(node.lineNumber,
                      std::dynamic_pointer_cast<VariableASTNode>
                          (node.getChildren()[0])->getValue());
}

void ModifiesExtractor::addModifiesRelation(int x, string var) {
  pkbWriter->addModifies(x, var);
}
