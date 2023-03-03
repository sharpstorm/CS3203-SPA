#include "SourceTokenParser.h"

#include <memory>

using std::make_unique;

SourceTokenParser::SourceTokenParser():
    entityParser(make_unique<EntityParser>()),
    exprParser(make_unique<ExpressionParser>(entityParser.get())),
    condParser(make_unique<ConditionalParser>(exprParser.get())),
    procedureParser(make_unique<ProcedureParser>(entityParser.get(),
                                                 exprParser.get(),
                                                 condParser.get())) {
}

AST SourceTokenParser::parseProcedure(vector<SourceToken> *tokens) {
  SourceParseState state(tokens);
  return AST(procedureParser->parse(&state));
}

AST SourceTokenParser::parseExpression(vector<SourceToken> *tokens) {
  SourceParseState state(tokens);
  return AST(exprParser->parse(&state));
}
