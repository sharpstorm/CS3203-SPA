#include "EntityParser.h"

#include <memory>

#include "sp/ast/entity/ConstantASTNode.h"
#include "sp/ast/entity/VariableASTNode.h"

using std::make_unique;

ASTNodePtr EntityParser::parseConstant(SourceParseState *state) const {
  SourceToken *token = state->expect(SIMPLE_TOKEN_INTEGER);
  return makeNode<ConstantASTNode>(token);
}

ASTNodePtr EntityParser::parseVariable(SourceParseState *state) const {
  SourceToken *token = state->expectVarchar();
  return makeNode<VariableASTNode>(token);
}

template<class T>
ASTNodePtr EntityParser::makeNode(const SourceToken *token) const {
  return make_unique<T>(token->getValue());
}
