#include "CallNode.h"

CallNode::CallNode(int lineNumber, string pName) :
    StatementASTNode(ASTNODE_CALL, "", lineNumber), procName(pName) {
}

void CallNode::accept(Extractor* e) {
  e->visitCall(this);
}

string CallNode::getName() {
  return procName;
}

string CallNode::toString() {
  return ":call";
}
