#pragma once

#include "catch.hpp"

#include <unordered_map>
#include <utility>

#include "../../util/PQLTestTokenSequenceBuilder.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/parser/token_parser/QueryTokenParseState.h"
#include "qps/parser/builder/QueryBuilder.h"
#include "../../SourceParserStub.h"
#include "qps/parser/lexer/QueryLexerFactory.h"
#include "qps/parser/token_parser/QueryTokenParser.h"

using std::unordered_map, std::move;

template <class Context>
QueryBuilderPtr testParsing(vector<PQLToken> dummyStream) {
  Context context;

  QueryTokenParseState state(&dummyStream);
  auto builder = make_unique<QueryBuilder>();
  context.parse(&state, builder.get());

  return std::move(builder);
}

template <class Context>
void testParsing(vector<PQLToken> dummyStream, QueryBuilder* builder) {
  Context context;

  QueryTokenParseState state(&dummyStream);
  context.parse(&state, builder);
}

template <class ParseContext, class ClauseType>
void testSuchThatParsing(vector<PQLToken> inputs,
                         unordered_map<string, PQLSynonymType> synonyms) {
  ParseContext context;
  QueryTokenParseState state(&inputs);
  QueryBuilder builder;

  for (auto it : synonyms) {
    builder.addSynonym(it.first, it.second);
  }

  auto clause = context.parse(&state, &builder);
  builder.addSuchThat(std::move(clause));

  auto clauses = builder.build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<ClauseType*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
}

template <class Context>
void testDeclarationParsing(vector<PQLToken> dummyStream, PQLSynonymName syn) {
  auto builder = testParsing<Context>(dummyStream);

  REQUIRE(builder->hasSynonym(syn));
}

