#include "EntityParser.h"

#include <memory>
#include <string>

#include "sp/ast/entity/ConstantASTNode.h"
#include "sp/ast/entity/VariableASTNode.h"

using std::make_unique, std::string;

ASTNodePtr EntityParser::parseConstant(SourceParseState *state) {
  SourceToken* token = state->expect(SIMPLE_TOKEN_INTEGER);
  string value = token->getValue();
  return make_unique<ConstantASTNode>(value);
}

ASTNodePtr EntityParser::parseVariable(SourceParseState *state) {
  SourceToken* token = state->expectVarchar();
  string value = token->getValue();
  return make_unique<VariableASTNode>(value);
}
