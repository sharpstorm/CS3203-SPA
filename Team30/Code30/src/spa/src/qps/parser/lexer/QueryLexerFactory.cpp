#include "QueryLexerFactory.h"

#include <memory>

using std::make_unique;

QueryLexerPtr QueryLexerFactory::makeLexer(const PQLQueryString *query) {
  return make_unique<QueryLexer>(query, &tokenTable);
}
