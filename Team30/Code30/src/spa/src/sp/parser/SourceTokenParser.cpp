#include <memory>
#include <utility>

#include "SourceTokenParser.h"
#include "sp/ast/entity/ProgramNode.h"

using std::make_unique;

SourceTokenParser::SourceTokenParser() :
    entityParser(make_unique<EntityParser>()),
    exprParser(make_unique<ExpressionParser>(entityParser.get())),
    condParser(make_unique<ConditionalParser>(exprParser.get())),
    procedureParser(make_unique<ProcedureParser>(entityParser.get(),
                                                 exprParser.get(),
                                                 condParser.get())) {
}

ASTPtr SourceTokenParser::parseProgram(SourceTokenStream *tokens) const {
  SourceParseState state(tokens);
  ASTNodePtr programNode = make_unique<ProgramNode>();
  programNode->addChild(std::move(procedureParser->parse(&state)));
  while (!state.isEnd()) {
    programNode->addChild(std::move(procedureParser->parse(&state)));
  }
  return make_unique<AST>(std::move(programNode));
}

ASTPtr SourceTokenParser::parseExpression(SourceTokenStream *tokens) const {
  SourceParseState state(tokens);
  ASTPtr ret = make_unique<AST>(std::move(exprParser->parse(&state)));
  if (!state.isEnd()) {
    throw SPError(SPERR_UNEXPECTED_TOKEN);
  }
  return ret;
}
