#include "EntityParser.h"

#include <memory>
#include <string>

#include "common/ast/entity/ConstantASTNode.h"
#include "common/ast/entity/VariableASTNode.h"

using std::make_shared, std::string;

ASTNodePtr EntityParser::parseConstant(SourceParseState *state) {
  SourceToken* token = state->expect(SIMPLE_TOKEN_INTEGER);
  string value = token->getValue();
  ASTNodePtr newNode = make_shared<ConstantASTNode>(value);
  if (!state->hasCached()) {
    state->setCached(newNode);
  }

  return newNode;
}

ASTNodePtr EntityParser::parseVariable(SourceParseState *state) {
  SourceToken* token = state->expectVarchar();
  string value = token->getValue();
  ASTNodePtr newNode = make_shared<VariableASTNode>(value);
  return newNode;
}
