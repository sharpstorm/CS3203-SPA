#pragma once

#include <string>
#include <memory>
#include "IQueryParser.h"
#include "lexer/QueryLexer.h"
#include "token_parser/QueryTokenParser.h"

using std::string;

class QueryParser: public IQueryParser {
 public:
  unique_ptr<PQLQuery> parseQuery(string* query);

 private:
  QueryLexer lexer;
};
