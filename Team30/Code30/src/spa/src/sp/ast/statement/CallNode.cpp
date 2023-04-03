#include "CallNode.h"

CallNode::CallNode(LineNumber line, string pName) :
    StatementASTNode(ASTNODE_CALL, "", line), procName(pName) {
}

void CallNode::accept(IExtractor* e) {
  e->visitCall(this);
}

string CallNode::getName() {
  return procName;
}
