#include "VariableASTNode.h"
#include "sp/extractor/IExtractor.h"

VariableASTNode::VariableASTNode(string v): ASTNode(ASTNODE_VARIABLE, v) {}

void VariableASTNode::accept(IExtractor* e) {
  e->visitVariable(this);
}
