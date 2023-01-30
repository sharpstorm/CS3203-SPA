#pragma once

#include <string>
#include "IQueryParser.h"

using std::string;

class QueryParser: public IQueryParser {
 public:
  PQLQuery* parseQuery(string* query);
};
