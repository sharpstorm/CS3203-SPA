#include "CFGCreator.h"

#include "CFG.h"

CFG CFGCreator::createCFG(AST ast) { return DFS(ast.getMutableRoot()); }

CFG CFGCreator::DFS(ASTNodePtr node) {
  CFG cfg = CFG(node->getValue());  // todo: to be changed

  if (node->getChildren().size() > 0) {
    for (ASTNodePtr child : node->getChildren()) {
      if (child != nullptr) {
        childNode = addChild(child);
        DFS(child);
      }
    }
  }
}

void addChild(AssignNode* node) {
  CFGNode childNode = CFGNode(CFG_NORMAL_BLOCK, node->getLineNumber());
}
