#include <string>
#include "StatementASTNode.h"

using std::string;

StatementASTNode::StatementASTNode(ASTNodeType type, string value) : ASTNode(type, value){

}

void StatementASTNode::accept(shared_ptr<Extractor> e) {}
