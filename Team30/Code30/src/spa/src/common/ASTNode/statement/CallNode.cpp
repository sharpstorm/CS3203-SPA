#include <string>
#include <vector>
#include <memory>
#include "CallNode.h"

using std::vector, std::shared_ptr, std::string;

CallNode::CallNode(int lineNumber, string pName) :
    StatementASTNode(ASTNODE_CALL, "", lineNumber), procName(pName) {
}

void CallNode::accept(Extractor* e) {
  e->visit(this);
}

string CallNode::toString() {
  return ":call";
}
