#include <string>
#include "StatementASTNode.h"

using std::string;

StatementASTNode::StatementASTNode(ASTNodeType type, string value,
                                   int lineNumber) :
    ASTNode(type, value), lineNumber(lineNumber) {
}
