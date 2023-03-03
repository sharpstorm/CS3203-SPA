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

  // True result
  query = "assign a; variable v; Select BOOLEAN such that Uses(a, v) pattern a(v,_)";
  expectedRes = unordered_set<string>({"TRUE"});
  launchQuery(qps.get(), query, expectedRes);

  // False result
  query = "stmt s; assign a; Select BOOLEAN such that Follows(s, s)";
  expectedRes = unordered_set<string>{ "FALSE" };
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
