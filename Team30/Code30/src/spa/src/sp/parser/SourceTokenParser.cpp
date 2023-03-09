#include <memory>
#include "SourceTokenParser.h"
#include "sp/ast/entity/ProgramNode.h"

using std::make_unique;

SourceTokenParser::SourceTokenParser():
    entityParser(make_unique<EntityParser>()),
    exprParser(make_unique<ExpressionParser>(entityParser.get())),
    condParser(make_unique<ConditionalParser>(exprParser.get())),
    procedureParser(make_unique<ProcedureParser>(entityParser.get(),
                                                 exprParser.get(),
                                                 condParser.get())) {
}

ASTPtr SourceTokenParser::parseProgram(vector<SourceToken> *tokens) {
  SourceParseState state(tokens);
  ASTNodePtr programNode = make_unique<ProgramNode>();
  programNode->addChild(std::move(procedureParser->parse(&state)));
  while (!state.isEnd()) {
    programNode->addChild(std::move(procedureParser->parse(&state)));
  }
  return make_unique<AST>(std::move(programNode));
}

ASTPtr SourceTokenParser::parseExpression(vector<SourceToken> *tokens) {
  SourceParseState state(tokens);
  return make_unique<AST>(std::move(exprParser->parse(&state)));
}
