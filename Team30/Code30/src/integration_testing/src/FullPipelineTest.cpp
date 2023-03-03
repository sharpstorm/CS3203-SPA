#include "catch.hpp"

#include <memory>
#include <unordered_set>
#include "qps/QPSFacade.h"
#include "pkb/writers/PkbWriter.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "sp/SpDriver.h"
#include "TestUtils.h"

using std::make_unique, std::make_shared, std::unordered_set, std::to_string;

TEST_CASE("Test Full End-to-end") {
  auto pkb = make_unique<PKB>();
  auto pkbWriter = make_unique<PkbWriter>(pkb.get());
  shared_ptr<PkbQueryHandler> pkbQH_ptr = make_shared<PkbQueryHandler>(pkb.get());

  unique_ptr<IQPS> qps = make_unique<QPSFacade>(pkbQH_ptr);
  SpDriver spDriver;

  spDriver.parseSource("procedure Example {\n"
                       "  x = 2;\n" // 1
                       "  z = 3;\n" // 2
                       "  i = 5;\n" // 3
                       "  while (i!=0) {\n" // 4
                       "    x = x - 1;\n" // 5
                       "    if (x==1) then {\n" // 6
                       "      z = x + 1; }\n" // 7
                       "    else {\n"
                       "      y = z + x; }\n" // 8
                       "    z = z + x + i;\n" // 9
                       "    read x;\n" // 10
                       "    i = i - 1; }\n" // 11
                       "  z = x + 1; }" // 12
      , pkbWriter.get());

  string query;
  unordered_set<string> expectedRes;


  query = "assign a1; Select a1 such that Follows(1,2)";
  expectedRes = unordered_set<string>({"1", "2", "3", "5", "7", "8", "9", "11", "12"});
  launchQuery(qps.get(), query, expectedRes);

  // All synonyms are valid
  query = "assign a1, a2; variable v; Select <a1,v,a2> such that Uses(a1, v) pattern a1(v,_) such that Follows(1,a2)";
  expectedRes = unordered_set<string>({"5 x 2", "9 z 2", "11 i 2"});
  launchQuery(qps.get(), query, expectedRes);

  // Ordering preserved for the same query
  query = "assign a1, a2; variable v; Select <v,a1,a2> such that Uses(a1, v) pattern a1(v,_) such that Follows(1,a2)";
  expectedRes = unordered_set<string>({"x 5 2", "z 9 2", "i 11 2"});
  launchQuery(qps.get(), query, expectedRes);

  // One synonym has empty result
  query = "assign a1, a2; variable v; Select <v,a1,a2> such that Uses(a1, v) pattern a1(v,_) such that Follows(a2,a2)";
  expectedRes = unordered_set<string>({});
  launchQuery(qps.get(), query, expectedRes);

  // One synonym is a pure select
  query = "assign a1; variable v, v2; Select <v,a1,v2> such that Uses(a1, v) pattern a1(v,_)";
  expectedRes = unordered_set<string>({"x 5 z", "x 5 x", "x 5 i", "x 5 y",
                                       "z 9 z", "z 9 x", "z 9 i", "z 9 y",
                                       "i 11 z", "i 11 x", "i 11 i", "i 11 y"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a1; variable v, v2; Select <v,a1> such that Uses(a1, v)";
  expectedRes = unordered_set<string>({"x 5", "x 7", "z 8", "x 8", "z 9", "x 9", "i 9", "i 11", "x 12"});
  launchQuery(qps.get(), query, expectedRes);

  // Tupling with tables
  query = "assign a1,a2; variable v1, v2; Select <a1, v1, a2, v2> such that Uses(a1, v1) such that Modifies(a2, v2)";
  expectedRes = unordered_set<string>({"5 x 1 x", "5 x 2 z", "5 x 3 i", "5 x 5 x", "5 x 7 z", "5 x 8 y", "5 x 9 z", "5 x 11 i", "5 x 12 z", "7 x 1 x", "7 x 2 z", "7 x 3 i", "7 x 5 x", "7 x 7 z", "7 x 8 y", "7 x 9 z", "7 x 11 i", "7 x 12 z", "8 z 1 x", "8 z 2 z", "8 z 3 i", "8 z 5 x", "8 z 7 z", "8 z 8 y", "8 z 9 z", "8 z 11 i", "8 z 12 z", "8 x 1 x", "8 x 2 z", "8 x 3 i", "8 x 5 x", "8 x 7 z", "8 x 8 y", "8 x 9 z", "8 x 11 i", "8 x 12 z", "9 z 1 x", "9 z 2 z", "9 z 3 i", "9 z 5 x", "9 z 7 z", "9 z 8 y", "9 z 9 z", "9 z 11 i", "9 z 12 z", "9 x 1 x", "9 x 2 z", "9 x 3 i", "9 x 5 x", "9 x 7 z", "9 x 8 y", "9 x 9 z", "9 x 11 i", "9 x 12 z", "9 i 1 x", "9 i 2 z", "9 i 3 i", "9 i 5 x", "9 i 7 z", "9 i 8 y", "9 i 9 z", "9 i 11 i", "9 i 12 z", "11 i 1 x", "11 i 2 z", "11 i 3 i", "11 i 5 x", "11 i 7 z", "11 i 8 y", "11 i 9 z", "11 i 11 i", "11 i 12 z", "12 x 1 x", "12 x 2 z", "12 x 3 i", "12 x 5 x", "12 x 7 z", "12 x 8 y", "12 x 9 z", "12 x 11 i", "12 x 12 z"});
  launchQuery(qps.get(), query, expectedRes);

  // Pure select tupling
  query = "assign a1,a2; variable v1, v2; Select <a2, v2> such that Uses(a1, v1)";
  expectedRes = unordered_set<string>({"1 z", "1 x", "1 i", "1 y",
                                       "2 z", "2 x", "2 i", "2 y",
                                       "3 z", "3 x", "3 i", "3 y",
                                       "5 z", "5 x", "5 i", "5 y",
                                       "7 z", "7 x", "7 i", "7 y",
                                       "8 z", "8 x", "8 i", "8 y",
                                       "9 z", "9 x", "9 i", "9 y",
                                       "11 z", "11 x", "11 i", "11 y",
                                       "12 z", "12 x", "12 i", "12 y"});
  launchQuery(qps.get(), query, expectedRes);

  // Pure select tupling but other clause fails
  query = "assign a1,a2; variable v2; Select <a2, v2> pattern a1 (_,_\"9\"_)";
  expectedRes = unordered_set<string>({});
  launchQuery(qps.get(), query, expectedRes);

  // ONLY select clause tupling
  query = "assign a1,a2; variable v2; Select <a1, a2>";
  expectedRes = unordered_set<string>({"1 1", "1 2", "1 3", "1 5", "1 7", "1 8", "1 9", "1 11", "1 12",
                                       "2 1", "2 2", "2 3", "2 5", "2 7", "2 8", "2 9", "2 11", "2 12",
                                       "3 1", "3 2", "3 3", "3 5", "3 7", "3 8", "3 9", "3 11", "3 12",
                                       "5 1", "5 2", "5 3", "5 5", "5 7", "5 8", "5 9", "5 11", "5 12",
                                       "7 1", "7 2", "7 3", "7 5", "7 7", "7 8", "7 9", "7 11", "7 12",
                                       "8 1", "8 2", "8 3", "8 5", "8 7", "8 8", "8 9", "8 11", "8 12",
                                       "9 1", "9 2", "9 3", "9 5", "9 7", "9 8", "9 9", "9 11", "9 12",
                                       "11 1", "11 2", "11 3", "11 5", "11 7", "11 8", "11 9", "11 11", "11 12",
                                       "12 1", "12 2", "12 3", "12 5", "12 7", "12 8", "12 9", "12 11", "12 12"});
  launchQuery(qps.get(), query, expectedRes);

  // Stress test tupling (Pure Select)
  query = "assign a1,a2; variable v1,v2; Select <a1, a2, v1, v2>";
  expectedRes = unordered_set<string>({"1 1 z z", "1 1 z x", "1 1 z i", "1 1 z y", "1 1 x z", "1 1 x x", "1 1 x i", "1 1 x y", "1 1 i z", "1 1 i x", "1 1 i i", "1 1 i y", "1 1 y z", "1 1 y x", "1 1 y i", "1 1 y y", "1 2 z z", "1 2 z x", "1 2 z i", "1 2 z y", "1 2 x z", "1 2 x x", "1 2 x i", "1 2 x y", "1 2 i z", "1 2 i x", "1 2 i i", "1 2 i y", "1 2 y z", "1 2 y x", "1 2 y i", "1 2 y y", "1 3 z z", "1 3 z x", "1 3 z i", "1 3 z y", "1 3 x z", "1 3 x x", "1 3 x i", "1 3 x y", "1 3 i z", "1 3 i x", "1 3 i i", "1 3 i y", "1 3 y z", "1 3 y x", "1 3 y i", "1 3 y y", "1 5 z z", "1 5 z x", "1 5 z i", "1 5 z y", "1 5 x z", "1 5 x x", "1 5 x i", "1 5 x y", "1 5 i z", "1 5 i x", "1 5 i i", "1 5 i y", "1 5 y z", "1 5 y x", "1 5 y i", "1 5 y y", "1 7 z z", "1 7 z x", "1 7 z i", "1 7 z y", "1 7 x z", "1 7 x x", "1 7 x i", "1 7 x y", "1 7 i z", "1 7 i x", "1 7 i i", "1 7 i y", "1 7 y z", "1 7 y x", "1 7 y i", "1 7 y y", "1 8 z z", "1 8 z x", "1 8 z i", "1 8 z y", "1 8 x z", "1 8 x x", "1 8 x i", "1 8 x y", "1 8 i z", "1 8 i x", "1 8 i i", "1 8 i y", "1 8 y z", "1 8 y x", "1 8 y i", "1 8 y y", "1 9 z z", "1 9 z x", "1 9 z i", "1 9 z y", "1 9 x z", "1 9 x x", "1 9 x i", "1 9 x y", "1 9 i z", "1 9 i x", "1 9 i i", "1 9 i y", "1 9 y z", "1 9 y x", "1 9 y i", "1 9 y y", "1 11 z z", "1 11 z x", "1 11 z i", "1 11 z y", "1 11 x z", "1 11 x x", "1 11 x i", "1 11 x y", "1 11 i z", "1 11 i x", "1 11 i i", "1 11 i y", "1 11 y z", "1 11 y x", "1 11 y i", "1 11 y y", "1 12 z z", "1 12 z x", "1 12 z i", "1 12 z y", "1 12 x z", "1 12 x x", "1 12 x i", "1 12 x y", "1 12 i z", "1 12 i x", "1 12 i i", "1 12 i y", "1 12 y z", "1 12 y x", "1 12 y i", "1 12 y y", "2 1 z z", "2 1 z x", "2 1 z i", "2 1 z y", "2 1 x z", "2 1 x x", "2 1 x i", "2 1 x y", "2 1 i z", "2 1 i x", "2 1 i i", "2 1 i y", "2 1 y z", "2 1 y x", "2 1 y i", "2 1 y y", "2 2 z z", "2 2 z x", "2 2 z i", "2 2 z y", "2 2 x z", "2 2 x x", "2 2 x i", "2 2 x y", "2 2 i z", "2 2 i x", "2 2 i i", "2 2 i y", "2 2 y z", "2 2 y x", "2 2 y i", "2 2 y y", "2 3 z z", "2 3 z x", "2 3 z i", "2 3 z y", "2 3 x z", "2 3 x x", "2 3 x i", "2 3 x y", "2 3 i z", "2 3 i x", "2 3 i i", "2 3 i y", "2 3 y z", "2 3 y x", "2 3 y i", "2 3 y y", "2 5 z z", "2 5 z x", "2 5 z i", "2 5 z y", "2 5 x z", "2 5 x x", "2 5 x i", "2 5 x y", "2 5 i z", "2 5 i x", "2 5 i i", "2 5 i y", "2 5 y z", "2 5 y x", "2 5 y i", "2 5 y y", "2 7 z z", "2 7 z x", "2 7 z i", "2 7 z y", "2 7 x z", "2 7 x x", "2 7 x i", "2 7 x y", "2 7 i z", "2 7 i x", "2 7 i i", "2 7 i y", "2 7 y z", "2 7 y x", "2 7 y i", "2 7 y y", "2 8 z z", "2 8 z x", "2 8 z i", "2 8 z y", "2 8 x z", "2 8 x x", "2 8 x i", "2 8 x y", "2 8 i z", "2 8 i x", "2 8 i i", "2 8 i y", "2 8 y z", "2 8 y x", "2 8 y i", "2 8 y y", "2 9 z z", "2 9 z x", "2 9 z i", "2 9 z y", "2 9 x z", "2 9 x x", "2 9 x i", "2 9 x y", "2 9 i z", "2 9 i x", "2 9 i i", "2 9 i y", "2 9 y z", "2 9 y x", "2 9 y i", "2 9 y y", "2 11 z z", "2 11 z x", "2 11 z i", "2 11 z y", "2 11 x z", "2 11 x x", "2 11 x i", "2 11 x y", "2 11 i z", "2 11 i x", "2 11 i i", "2 11 i y", "2 11 y z", "2 11 y x", "2 11 y i", "2 11 y y", "2 12 z z", "2 12 z x", "2 12 z i", "2 12 z y", "2 12 x z", "2 12 x x", "2 12 x i", "2 12 x y", "2 12 i z", "2 12 i x", "2 12 i i", "2 12 i y", "2 12 y z", "2 12 y x", "2 12 y i", "2 12 y y", "3 1 z z", "3 1 z x", "3 1 z i", "3 1 z y", "3 1 x z", "3 1 x x", "3 1 x i", "3 1 x y", "3 1 i z", "3 1 i x", "3 1 i i", "3 1 i y", "3 1 y z", "3 1 y x", "3 1 y i", "3 1 y y", "3 2 z z", "3 2 z x", "3 2 z i", "3 2 z y", "3 2 x z", "3 2 x x", "3 2 x i", "3 2 x y", "3 2 i z", "3 2 i x", "3 2 i i", "3 2 i y", "3 2 y z", "3 2 y x", "3 2 y i", "3 2 y y", "3 3 z z", "3 3 z x", "3 3 z i", "3 3 z y", "3 3 x z", "3 3 x x", "3 3 x i", "3 3 x y", "3 3 i z", "3 3 i x", "3 3 i i", "3 3 i y", "3 3 y z", "3 3 y x", "3 3 y i", "3 3 y y", "3 5 z z", "3 5 z x", "3 5 z i", "3 5 z y", "3 5 x z", "3 5 x x", "3 5 x i", "3 5 x y", "3 5 i z", "3 5 i x", "3 5 i i", "3 5 i y", "3 5 y z", "3 5 y x", "3 5 y i", "3 5 y y", "3 7 z z", "3 7 z x", "3 7 z i", "3 7 z y", "3 7 x z", "3 7 x x", "3 7 x i", "3 7 x y", "3 7 i z", "3 7 i x", "3 7 i i", "3 7 i y", "3 7 y z", "3 7 y x", "3 7 y i", "3 7 y y", "3 8 z z", "3 8 z x", "3 8 z i", "3 8 z y", "3 8 x z", "3 8 x x", "3 8 x i", "3 8 x y", "3 8 i z", "3 8 i x", "3 8 i i", "3 8 i y", "3 8 y z", "3 8 y x", "3 8 y i", "3 8 y y", "3 9 z z", "3 9 z x", "3 9 z i", "3 9 z y", "3 9 x z", "3 9 x x", "3 9 x i", "3 9 x y", "3 9 i z", "3 9 i x", "3 9 i i", "3 9 i y", "3 9 y z", "3 9 y x", "3 9 y i", "3 9 y y", "3 11 z z", "3 11 z x", "3 11 z i", "3 11 z y", "3 11 x z", "3 11 x x", "3 11 x i", "3 11 x y", "3 11 i z", "3 11 i x", "3 11 i i", "3 11 i y", "3 11 y z", "3 11 y x", "3 11 y i", "3 11 y y", "3 12 z z", "3 12 z x", "3 12 z i", "3 12 z y", "3 12 x z", "3 12 x x", "3 12 x i", "3 12 x y", "3 12 i z", "3 12 i x", "3 12 i i", "3 12 i y", "3 12 y z", "3 12 y x", "3 12 y i", "3 12 y y", "5 1 z z", "5 1 z x", "5 1 z i", "5 1 z y", "5 1 x z", "5 1 x x", "5 1 x i", "5 1 x y", "5 1 i z", "5 1 i x", "5 1 i i", "5 1 i y", "5 1 y z", "5 1 y x", "5 1 y i", "5 1 y y", "5 2 z z", "5 2 z x", "5 2 z i", "5 2 z y", "5 2 x z", "5 2 x x", "5 2 x i", "5 2 x y", "5 2 i z", "5 2 i x", "5 2 i i", "5 2 i y", "5 2 y z", "5 2 y x", "5 2 y i", "5 2 y y", "5 3 z z", "5 3 z x", "5 3 z i", "5 3 z y", "5 3 x z", "5 3 x x", "5 3 x i", "5 3 x y", "5 3 i z", "5 3 i x", "5 3 i i", "5 3 i y", "5 3 y z", "5 3 y x", "5 3 y i", "5 3 y y", "5 5 z z", "5 5 z x", "5 5 z i", "5 5 z y", "5 5 x z", "5 5 x x", "5 5 x i", "5 5 x y", "5 5 i z", "5 5 i x", "5 5 i i", "5 5 i y", "5 5 y z", "5 5 y x", "5 5 y i", "5 5 y y", "5 7 z z", "5 7 z x", "5 7 z i", "5 7 z y", "5 7 x z", "5 7 x x", "5 7 x i", "5 7 x y", "5 7 i z", "5 7 i x", "5 7 i i", "5 7 i y", "5 7 y z", "5 7 y x", "5 7 y i", "5 7 y y", "5 8 z z", "5 8 z x", "5 8 z i", "5 8 z y", "5 8 x z", "5 8 x x", "5 8 x i", "5 8 x y", "5 8 i z", "5 8 i x", "5 8 i i", "5 8 i y", "5 8 y z", "5 8 y x", "5 8 y i", "5 8 y y", "5 9 z z", "5 9 z x", "5 9 z i", "5 9 z y", "5 9 x z", "5 9 x x", "5 9 x i", "5 9 x y", "5 9 i z", "5 9 i x", "5 9 i i", "5 9 i y", "5 9 y z", "5 9 y x", "5 9 y i", "5 9 y y", "5 11 z z", "5 11 z x", "5 11 z i", "5 11 z y", "5 11 x z", "5 11 x x", "5 11 x i", "5 11 x y", "5 11 i z", "5 11 i x", "5 11 i i", "5 11 i y", "5 11 y z", "5 11 y x", "5 11 y i", "5 11 y y", "5 12 z z", "5 12 z x", "5 12 z i", "5 12 z y", "5 12 x z", "5 12 x x", "5 12 x i", "5 12 x y", "5 12 i z", "5 12 i x", "5 12 i i", "5 12 i y", "5 12 y z", "5 12 y x", "5 12 y i", "5 12 y y", "7 1 z z", "7 1 z x", "7 1 z i", "7 1 z y", "7 1 x z", "7 1 x x", "7 1 x i", "7 1 x y", "7 1 i z", "7 1 i x", "7 1 i i", "7 1 i y", "7 1 y z", "7 1 y x", "7 1 y i", "7 1 y y", "7 2 z z", "7 2 z x", "7 2 z i", "7 2 z y", "7 2 x z", "7 2 x x", "7 2 x i", "7 2 x y", "7 2 i z", "7 2 i x", "7 2 i i", "7 2 i y", "7 2 y z", "7 2 y x", "7 2 y i", "7 2 y y", "7 3 z z", "7 3 z x", "7 3 z i", "7 3 z y", "7 3 x z", "7 3 x x", "7 3 x i", "7 3 x y", "7 3 i z", "7 3 i x", "7 3 i i", "7 3 i y", "7 3 y z", "7 3 y x", "7 3 y i", "7 3 y y", "7 5 z z", "7 5 z x", "7 5 z i", "7 5 z y", "7 5 x z", "7 5 x x", "7 5 x i", "7 5 x y", "7 5 i z", "7 5 i x", "7 5 i i", "7 5 i y", "7 5 y z", "7 5 y x", "7 5 y i", "7 5 y y", "7 7 z z", "7 7 z x", "7 7 z i", "7 7 z y", "7 7 x z", "7 7 x x", "7 7 x i", "7 7 x y", "7 7 i z", "7 7 i x", "7 7 i i", "7 7 i y", "7 7 y z", "7 7 y x", "7 7 y i", "7 7 y y", "7 8 z z", "7 8 z x", "7 8 z i", "7 8 z y", "7 8 x z", "7 8 x x", "7 8 x i", "7 8 x y", "7 8 i z", "7 8 i x", "7 8 i i", "7 8 i y", "7 8 y z", "7 8 y x", "7 8 y i", "7 8 y y", "7 9 z z", "7 9 z x", "7 9 z i", "7 9 z y", "7 9 x z", "7 9 x x", "7 9 x i", "7 9 x y", "7 9 i z", "7 9 i x", "7 9 i i", "7 9 i y", "7 9 y z", "7 9 y x", "7 9 y i", "7 9 y y", "7 11 z z", "7 11 z x", "7 11 z i", "7 11 z y", "7 11 x z", "7 11 x x", "7 11 x i", "7 11 x y", "7 11 i z", "7 11 i x", "7 11 i i", "7 11 i y", "7 11 y z", "7 11 y x", "7 11 y i", "7 11 y y", "7 12 z z", "7 12 z x", "7 12 z i", "7 12 z y", "7 12 x z", "7 12 x x", "7 12 x i", "7 12 x y", "7 12 i z", "7 12 i x", "7 12 i i", "7 12 i y", "7 12 y z", "7 12 y x", "7 12 y i", "7 12 y y", "8 1 z z", "8 1 z x", "8 1 z i", "8 1 z y", "8 1 x z", "8 1 x x", "8 1 x i", "8 1 x y", "8 1 i z", "8 1 i x", "8 1 i i", "8 1 i y", "8 1 y z", "8 1 y x", "8 1 y i", "8 1 y y", "8 2 z z", "8 2 z x", "8 2 z i", "8 2 z y", "8 2 x z", "8 2 x x", "8 2 x i", "8 2 x y", "8 2 i z", "8 2 i x", "8 2 i i", "8 2 i y", "8 2 y z", "8 2 y x", "8 2 y i", "8 2 y y", "8 3 z z", "8 3 z x", "8 3 z i", "8 3 z y", "8 3 x z", "8 3 x x", "8 3 x i", "8 3 x y", "8 3 i z", "8 3 i x", "8 3 i i", "8 3 i y", "8 3 y z", "8 3 y x", "8 3 y i", "8 3 y y", "8 5 z z", "8 5 z x", "8 5 z i", "8 5 z y", "8 5 x z", "8 5 x x", "8 5 x i", "8 5 x y", "8 5 i z", "8 5 i x", "8 5 i i", "8 5 i y", "8 5 y z", "8 5 y x", "8 5 y i", "8 5 y y", "8 7 z z", "8 7 z x", "8 7 z i", "8 7 z y", "8 7 x z", "8 7 x x", "8 7 x i", "8 7 x y", "8 7 i z", "8 7 i x", "8 7 i i", "8 7 i y", "8 7 y z", "8 7 y x", "8 7 y i", "8 7 y y", "8 8 z z", "8 8 z x", "8 8 z i", "8 8 z y", "8 8 x z", "8 8 x x", "8 8 x i", "8 8 x y", "8 8 i z", "8 8 i x", "8 8 i i", "8 8 i y", "8 8 y z", "8 8 y x", "8 8 y i", "8 8 y y", "8 9 z z", "8 9 z x", "8 9 z i", "8 9 z y", "8 9 x z", "8 9 x x", "8 9 x i", "8 9 x y", "8 9 i z", "8 9 i x", "8 9 i i", "8 9 i y", "8 9 y z", "8 9 y x", "8 9 y i", "8 9 y y", "8 11 z z", "8 11 z x", "8 11 z i", "8 11 z y", "8 11 x z", "8 11 x x", "8 11 x i", "8 11 x y", "8 11 i z", "8 11 i x", "8 11 i i", "8 11 i y", "8 11 y z", "8 11 y x", "8 11 y i", "8 11 y y", "8 12 z z", "8 12 z x", "8 12 z i", "8 12 z y", "8 12 x z", "8 12 x x", "8 12 x i", "8 12 x y", "8 12 i z", "8 12 i x", "8 12 i i", "8 12 i y", "8 12 y z", "8 12 y x", "8 12 y i", "8 12 y y", "9 1 z z", "9 1 z x", "9 1 z i", "9 1 z y", "9 1 x z", "9 1 x x", "9 1 x i", "9 1 x y", "9 1 i z", "9 1 i x", "9 1 i i", "9 1 i y", "9 1 y z", "9 1 y x", "9 1 y i", "9 1 y y", "9 2 z z", "9 2 z x", "9 2 z i", "9 2 z y", "9 2 x z", "9 2 x x", "9 2 x i", "9 2 x y", "9 2 i z", "9 2 i x", "9 2 i i", "9 2 i y", "9 2 y z", "9 2 y x", "9 2 y i", "9 2 y y", "9 3 z z", "9 3 z x", "9 3 z i", "9 3 z y", "9 3 x z", "9 3 x x", "9 3 x i", "9 3 x y", "9 3 i z", "9 3 i x", "9 3 i i", "9 3 i y", "9 3 y z", "9 3 y x", "9 3 y i", "9 3 y y", "9 5 z z", "9 5 z x", "9 5 z i", "9 5 z y", "9 5 x z", "9 5 x x", "9 5 x i", "9 5 x y", "9 5 i z", "9 5 i x", "9 5 i i", "9 5 i y", "9 5 y z", "9 5 y x", "9 5 y i", "9 5 y y", "9 7 z z", "9 7 z x", "9 7 z i", "9 7 z y", "9 7 x z", "9 7 x x", "9 7 x i", "9 7 x y", "9 7 i z", "9 7 i x", "9 7 i i", "9 7 i y", "9 7 y z", "9 7 y x", "9 7 y i", "9 7 y y", "9 8 z z", "9 8 z x", "9 8 z i", "9 8 z y", "9 8 x z", "9 8 x x", "9 8 x i", "9 8 x y", "9 8 i z", "9 8 i x", "9 8 i i", "9 8 i y", "9 8 y z", "9 8 y x", "9 8 y i", "9 8 y y", "9 9 z z", "9 9 z x", "9 9 z i", "9 9 z y", "9 9 x z", "9 9 x x", "9 9 x i", "9 9 x y", "9 9 i z", "9 9 i x", "9 9 i i", "9 9 i y", "9 9 y z", "9 9 y x", "9 9 y i", "9 9 y y", "9 11 z z", "9 11 z x", "9 11 z i", "9 11 z y", "9 11 x z", "9 11 x x", "9 11 x i", "9 11 x y", "9 11 i z", "9 11 i x", "9 11 i i", "9 11 i y", "9 11 y z", "9 11 y x", "9 11 y i", "9 11 y y", "9 12 z z", "9 12 z x", "9 12 z i", "9 12 z y", "9 12 x z", "9 12 x x", "9 12 x i", "9 12 x y", "9 12 i z", "9 12 i x", "9 12 i i", "9 12 i y", "9 12 y z", "9 12 y x", "9 12 y i", "9 12 y y", "11 1 z z", "11 1 z x", "11 1 z i", "11 1 z y", "11 1 x z", "11 1 x x", "11 1 x i", "11 1 x y", "11 1 i z", "11 1 i x", "11 1 i i", "11 1 i y", "11 1 y z", "11 1 y x", "11 1 y i", "11 1 y y", "11 2 z z", "11 2 z x", "11 2 z i", "11 2 z y", "11 2 x z", "11 2 x x", "11 2 x i", "11 2 x y", "11 2 i z", "11 2 i x", "11 2 i i", "11 2 i y", "11 2 y z", "11 2 y x", "11 2 y i", "11 2 y y", "11 3 z z", "11 3 z x", "11 3 z i", "11 3 z y", "11 3 x z", "11 3 x x", "11 3 x i", "11 3 x y", "11 3 i z", "11 3 i x", "11 3 i i", "11 3 i y", "11 3 y z", "11 3 y x", "11 3 y i", "11 3 y y", "11 5 z z", "11 5 z x", "11 5 z i", "11 5 z y", "11 5 x z", "11 5 x x", "11 5 x i", "11 5 x y", "11 5 i z", "11 5 i x", "11 5 i i", "11 5 i y", "11 5 y z", "11 5 y x", "11 5 y i", "11 5 y y", "11 7 z z", "11 7 z x", "11 7 z i", "11 7 z y", "11 7 x z", "11 7 x x", "11 7 x i", "11 7 x y", "11 7 i z", "11 7 i x", "11 7 i i", "11 7 i y", "11 7 y z", "11 7 y x", "11 7 y i", "11 7 y y", "11 8 z z", "11 8 z x", "11 8 z i", "11 8 z y", "11 8 x z", "11 8 x x", "11 8 x i", "11 8 x y", "11 8 i z", "11 8 i x", "11 8 i i", "11 8 i y", "11 8 y z", "11 8 y x", "11 8 y i", "11 8 y y", "11 9 z z", "11 9 z x", "11 9 z i", "11 9 z y", "11 9 x z", "11 9 x x", "11 9 x i", "11 9 x y", "11 9 i z", "11 9 i x", "11 9 i i", "11 9 i y", "11 9 y z", "11 9 y x", "11 9 y i", "11 9 y y", "11 11 z z", "11 11 z x", "11 11 z i", "11 11 z y", "11 11 x z", "11 11 x x", "11 11 x i", "11 11 x y", "11 11 i z", "11 11 i x", "11 11 i i", "11 11 i y", "11 11 y z", "11 11 y x", "11 11 y i", "11 11 y y", "11 12 z z", "11 12 z x", "11 12 z i", "11 12 z y", "11 12 x z", "11 12 x x", "11 12 x i", "11 12 x y", "11 12 i z", "11 12 i x", "11 12 i i", "11 12 i y", "11 12 y z", "11 12 y x", "11 12 y i", "11 12 y y", "12 1 z z", "12 1 z x", "12 1 z i", "12 1 z y", "12 1 x z", "12 1 x x", "12 1 x i", "12 1 x y", "12 1 i z", "12 1 i x", "12 1 i i", "12 1 i y", "12 1 y z", "12 1 y x", "12 1 y i", "12 1 y y", "12 2 z z", "12 2 z x", "12 2 z i", "12 2 z y", "12 2 x z", "12 2 x x", "12 2 x i", "12 2 x y", "12 2 i z", "12 2 i x", "12 2 i i", "12 2 i y", "12 2 y z", "12 2 y x", "12 2 y i", "12 2 y y", "12 3 z z", "12 3 z x", "12 3 z i", "12 3 z y", "12 3 x z", "12 3 x x", "12 3 x i", "12 3 x y", "12 3 i z", "12 3 i x", "12 3 i i", "12 3 i y", "12 3 y z", "12 3 y x", "12 3 y i", "12 3 y y", "12 5 z z", "12 5 z x", "12 5 z i", "12 5 z y", "12 5 x z", "12 5 x x", "12 5 x i", "12 5 x y", "12 5 i z", "12 5 i x", "12 5 i i", "12 5 i y", "12 5 y z", "12 5 y x", "12 5 y i", "12 5 y y", "12 7 z z", "12 7 z x", "12 7 z i", "12 7 z y", "12 7 x z", "12 7 x x", "12 7 x i", "12 7 x y", "12 7 i z", "12 7 i x", "12 7 i i", "12 7 i y", "12 7 y z", "12 7 y x", "12 7 y i", "12 7 y y", "12 8 z z", "12 8 z x", "12 8 z i", "12 8 z y", "12 8 x z", "12 8 x x", "12 8 x i", "12 8 x y", "12 8 i z", "12 8 i x", "12 8 i i", "12 8 i y", "12 8 y z", "12 8 y x", "12 8 y i", "12 8 y y", "12 9 z z", "12 9 z x", "12 9 z i", "12 9 z y", "12 9 x z", "12 9 x x", "12 9 x i", "12 9 x y", "12 9 i z", "12 9 i x", "12 9 i i", "12 9 i y", "12 9 y z", "12 9 y x", "12 9 y i", "12 9 y y", "12 11 z z", "12 11 z x", "12 11 z i", "12 11 z y", "12 11 x z", "12 11 x x", "12 11 x i", "12 11 x y", "12 11 i z", "12 11 i x", "12 11 i i", "12 11 i y", "12 11 y z", "12 11 y x", "12 11 y i", "12 11 y y", "12 12 z z", "12 12 z x", "12 12 z i", "12 12 z y", "12 12 x z", "12 12 x x", "12 12 x i", "12 12 x y", "12 12 i z", "12 12 i x", "12 12 i i", "12 12 i y", "12 12 y z", "12 12 y x", "12 12 y i", "12 12 y y"});
  launchQuery(qps.get(), query, expectedRes);

  // Stress test tupling (Table x Selects)

  // Stress test tupling (Pure Tables)

  // True result - Queries with synonyms
  query = "assign a; variable v; Select BOOLEAN such that Uses(a, v) pattern a(v,_)";
  expectedRes = unordered_set<string>({"TRUE"});
  launchQuery(qps.get(), query, expectedRes);

  // True result - Static queries
  query = "assign a; variable v; Select BOOLEAN such that Follows(1,2)";
  expectedRes = unordered_set<string>({"TRUE"});
  launchQuery(qps.get(), query, expectedRes);

  // True result - Combination of synonym and static queries
  query = "assign a; variable v; Select BOOLEAN such that Uses(a, v) pattern a(v,_) such that Follows(1,2)";
  expectedRes = unordered_set<string>({"TRUE"});
  launchQuery(qps.get(), query, expectedRes);

  // False result - Static queries
  query = "assign a; variable v; Select BOOLEAN such that Modifies(1,\"z\")";
  expectedRes = unordered_set<string>({"FALSE"});
  launchQuery(qps.get(), query, expectedRes);

  // False result - Queries with synonyms
  query = "stmt s; assign a; Select BOOLEAN such that Follows(s, s)";
  expectedRes = unordered_set<string>{"FALSE"};
  launchQuery(qps.get(), query, expectedRes);

  // False result - Combination of synonyms and static queries (synonym failing)
  query = "stmt s; assign a; Select BOOLEAN such that Follows(s, s) such that Follows(1,2)";
  expectedRes = unordered_set<string>{"FALSE"};
  launchQuery(qps.get(), query, expectedRes);

  // False result - Combination of synonyms and static queries (static failing)
  query = "stmt s; assign a; Select BOOLEAN such that Follows(a, s) such that Modifies(1,\"z\")";
  expectedRes = unordered_set<string>{"FALSE"};
  launchQuery(qps.get(), query, expectedRes);

  query = "stmt s; assign a; Select <s,a> such that Follows(s, s)";
  expectedRes = unordered_set<string>{};
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a1, a2; Select a1 such that Follows(1, 2) pattern a2(_,_)";
  expectedRes = unordered_set<string>({"1", "2", "3", "5", "7", "8", "9", "11", "12"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a such that Follows(1, 2) pattern a(\"x\",_)";
  expectedRes = unordered_set<string>({"1", "5"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a such that Follows(a, 2) pattern a(\"x\",_)";
  expectedRes = unordered_set<string>({"1"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a such that Follows*(a, 4) pattern a(\"x\",_)";
  expectedRes = unordered_set<string>({"1"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a such that Follows*(a, 4) pattern a(\"i\",_)";
  expectedRes = unordered_set<string>({"3"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; variable v; Select a such that Uses(a, v) pattern a(v,_)";
  expectedRes = unordered_set<string>({"5", "9", "11"});
  launchQuery(qps.get(), query, expectedRes);

  query = "while w; assign a; variable v; Select w such that Uses(a, v) pattern a(v,\"a\")";
  expectedRes = unordered_set<string>({});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a such that Uses(a, \"x\")";
  expectedRes = unordered_set<string>({"5", "7", "8", "9", "12"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a such that Modifies(a, \"x\")";
  expectedRes = unordered_set<string>({"1", "5"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a pattern a (\"x\", _\"1\"_)";
  expectedRes = unordered_set<string>({"5"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a pattern a (\"y\", _\"x\"_)";
  expectedRes = unordered_set<string>({"8",});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a pattern a (\"x\", _\"9\"_)";
  expectedRes = unordered_set<string>({});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a pattern a (\"g\", _)";
  expectedRes = unordered_set<string>({});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a pattern a (\"g\", _)";
  expectedRes = unordered_set<string>({});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a pattern a (_, _\"1\"_)";
  expectedRes = unordered_set<string>({"5", "7", "11", "12"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a pattern a (_, _\"x\"_)";
  expectedRes = unordered_set<string>({"5", "7", "8", "9", "12"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; Select a pattern a (_, _)";
  expectedRes = unordered_set<string>({"1", "2", "3", "5", "7", "8", "9", "11", "12"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; variable v; Select a pattern a (v, _)";
  expectedRes = unordered_set<string>({"1", "2", "3", "5", "7", "8", "9", "11", "12"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; variable v; Select v pattern a (v, _)";
  expectedRes = unordered_set<string>({"x", "z", "i", "y"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; variable v; Select a pattern a (v, _\"1\"_)";
  expectedRes = unordered_set<string>({"5", "7", "11", "12"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; variable v; Select v pattern a (v, _\"1\"_)";
  expectedRes = unordered_set<string>({"z", "x", "i"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a; variable v; Select a pattern a (v, _\"z\"_)";
  expectedRes = unordered_set<string>({"8", "9"});
  launchQuery(qps.get(), query, expectedRes);

  query = "constant c; Select c";
  expectedRes = unordered_set<string>({"0", "1", "2", "3", "5"});
  launchQuery(qps.get(), query, expectedRes);

  query = "procedure p; Select p";
  expectedRes = unordered_set<string>({"Example"});
  launchQuery(qps.get(), query, expectedRes);

  query = "assign a1, a2; variable v; Select v such that Modifies(a1, v) pattern a2(\"a\", \"huehuehue\")";
  expectedRes = unordered_set<string>({});
  launchQuery(qps.get(), query, expectedRes);
}
