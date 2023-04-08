#include "catch.hpp"

#include <unordered_set>

#include "qps/common/VariableTable.h"

using std::unordered_set;

TEST_CASE("VariableTable - Get Referred Synonyms (Non-empty)") {
  VariableTable varTable;
  varTable.add("a", PQLQuerySynonym(PQL_SYN_TYPE_ASSIGN, "a"));
  varTable.add("p", PQLQuerySynonym(PQL_SYN_TYPE_PRINT, "p"));
  varTable.finalizeTable();

  unordered_set<PQLSynonymName> expected = unordered_set<PQLSynonymName>({"a", "p"});
  REQUIRE(expected == varTable.getReferredSynonyms());
}

TEST_CASE("VariableTable - Get Referred Synonyms (Empty)") {
  VariableTable varTable;
  varTable.finalizeTable();
  unordered_set<PQLSynonymName> expected = unordered_set<PQLSynonymName>();
  REQUIRE(expected == varTable.getReferredSynonyms());
}