#include "catch.hpp"

#include <memory>
#include <unordered_set>

#include "qps/errors/QPSParserSyntaxError.h"
#include "pipeline_tests/TestPipelineProvider.h"

using std::make_unique, std::make_shared, std::unordered_set, std::to_string;

TEST_CASE("End-to-end Tuples Test") {
  auto pipeline = TestPipelineProvider();

  // All synonyms are valid
  pipeline.query(
      "assign a1, a2; variable v; Select <a1,v,a2> such that Uses(a1, v) pattern a1(v,_) such that Follows(1,a2)",
      {"5 x 2", "9 z 2", "11 i 2"});

  // Ordering preserved for the same query
  pipeline.query(
      "assign a1, a2; variable v; Select <v,a1,a2> such that Uses(a1, v) pattern a1(v,_) such that Follows(1,a2)",
      {"x 5 2", "z 9 2", "i 11 2"});

  // One synonym has empty result
  pipeline.query(
      "assign a1, a2; variable v; Select <v,a1,a2> such that Uses(a1, v) pattern a1(v,_) such that Follows(a2,a2)",
      {});

  // One synonym is a pure select
  pipeline.query(
      "assign a1; variable v, v2; Select <v,a1,v2> such that Uses(a1, v) pattern a1(v,_)",
      {"x 5 z", "x 5 x", "x 5 i", "x 5 y", "z 9 z", "z 9 x", "z 9 i", "z 9 y",
       "i 11 z", "i 11 x", "i 11 i", "i 11 y"});

  // Tupling with tables (duplicate rows for group results)
  pipeline.query(
      "assign a1, a2; variable v; Select <a1,v> such that Uses(a1, v) such that Follows*(a1,a2)",
      {"5 x", "9 z", "9 x", "9 i"});

  pipeline.query(
      "assign a1, a2; variable v1,v2; Select <a1,a2,v1,v2> such that Follows*(a1, a2) such that Uses(a1, v1) such that Modifies(a2,v2)",
      {
          "5 11 x i",
          "5 9 x z",
          "9 11 z i",
          "9 11 x i",
          "9 11 i i"});

  pipeline.query(
      "assign a1, a2; variable v1,v2; Select <a2,v2> such that Follows*(a1, a2) such that Uses(a1, v1) such that Modifies(a2,v2)",
      {"11 i", "9 z"});

  pipeline.query(
      "stmt s; assign a1, a2; variable v1,v2; Select <a1,s> such that Follows*(a1, a2) such that Uses(a1, v1) such that Modifies(a2,v2)",
      {
          "5 1", "5 2", "5 3", "5 4", "5 5", "5 6", "5 7", "5 8", "5 9", "5 10",
          "5 11", "5 12",
          "9 1", "9 2", "9 3", "9 4", "9 5", "9 6", "9 7", "9 8", "9 9", "9 10",
          "9 11", "9 12"});

  // Pure select tupling
  pipeline.query(
      "assign a1,a2; variable v1, v2; Select <a2, v2> such that Uses(a1, v1)",
      {"1 z", "1 x", "1 i", "1 y",
       "2 z", "2 x", "2 i", "2 y",
       "3 z", "3 x", "3 i", "3 y",
       "5 z", "5 x", "5 i", "5 y",
       "7 z", "7 x", "7 i", "7 y",
       "8 z", "8 x", "8 i", "8 y",
       "9 z", "9 x", "9 i", "9 y",
       "11 z", "11 x", "11 i", "11 y",
       "12 z", "12 x", "12 i", "12 y"});

  // Pure select tupling but other clause fails
  pipeline.query(
      "assign a1,a2; variable v2; Select <a2, v2> pattern a1 (_,_\"9\"_)",
      {});

  // ONLY select clause tupling
  pipeline.query("assign a1,a2; variable v2; Select <a1, a2>",
                 {
                     "1 1", "1 2", "1 3", "1 5", "1 7", "1 8", "1 9", "1 11",
                     "1 12",
                     "2 1", "2 2", "2 3", "2 5", "2 7", "2 8", "2 9", "2 11",
                     "2 12",
                     "3 1", "3 2", "3 3", "3 5", "3 7", "3 8", "3 9", "3 11",
                     "3 12",
                     "5 1", "5 2", "5 3", "5 5", "5 7", "5 8", "5 9", "5 11",
                     "5 12",
                     "7 1", "7 2", "7 3", "7 5", "7 7", "7 8", "7 9", "7 11",
                     "7 12",
                     "8 1", "8 2", "8 3", "8 5", "8 7", "8 8", "8 9", "8 11",
                     "8 12",
                     "9 1", "9 2", "9 3", "9 5", "9 7", "9 8", "9 9", "9 11",
                     "9 12",
                     "11 1", "11 2", "11 3", "11 5", "11 7", "11 8", "11 9",
                     "11 11", "11 12",
                     "12 1", "12 2", "12 3", "12 5", "12 7", "12 8", "12 9",
                     "12 11", "12 12"});

  // Tuple with only single syn
  pipeline.query(
      "assign a1, a2; variable v; Select <a1> such that Uses(a1, v) pattern a1(v,_) such that Follows(1,a2)",
      {"5", "9", "11"});
}

TEST_CASE("End-to-End BOOLEAN Test") {
  auto pipeline = TestPipelineProvider();

  // True result - Queries with synonyms
  pipeline.query(
      "assign a; variable v; Select BOOLEAN such that Uses(a, v) pattern a(v,_)",
      {"TRUE"});

  // True result - Static queries
  pipeline.query("assign a; variable v; Select BOOLEAN such that Follows(1,2)",
                 {"TRUE"});

  // True result - Combination of synonym and static queries
  pipeline.query(
      "assign a; variable v; Select BOOLEAN such that Uses(a, v) pattern a(v,_) such that Follows(1,2)",
      {"TRUE"});

  // False result - Static queries
  pipeline.query(
      "assign a; variable v; Select BOOLEAN such that Modifies(1,\"z\")",
      {"FALSE"});

  // False result - Queries with synonyms
  pipeline.query("stmt s; assign a; Select BOOLEAN such that Follows(s, s)",
                 {"FALSE"});

  // False result - Combination of synonyms and static queries (synonym failing)
  pipeline.query(
      "stmt s; assign a; Select BOOLEAN such that Follows(s, s) such that Follows(1,2)",
      {"FALSE"});

  // False result - Combination of synonyms and static queries (static failing)
  pipeline.query(
      "stmt s; assign a; Select BOOLEAN such that Follows(a, s) such that Modifies(1,\"z\")",
      {"FALSE"});

  // SAME AS ABOVE but uses 'and'
  pipeline.query(
      "stmt s; assign a; Select BOOLEAN such that Follows(a, s) and Modifies(1,\"z\")",
      {"FALSE"});

  // Declares BOOLEAN as a synonym
  pipeline.query("read BOOLEAN; Select BOOLEAN", {"10"});

  // Declares and uses BOOLEAN as a synonym
  pipeline.query("stmt BOOLEAN; Select BOOLEAN such that Follows(BOOLEAN, 2)",
                 {"1"});

  // Case-sensitive BOOLEAN keyword
  pipeline.query("stmt BOOLEAn; Select BOOLEAN such that Follows(BOOLEAn, 2)",
                 {"TRUE"});

  // No clause, non-empty result BOOLEAN
  pipeline.query("stmt s; Select BOOLEAN",
                 {"TRUE"});

  // No clause, empty result BOOLEAN
  pipeline.query("call cl; Select BOOLEAN",
                 {"TRUE"});
}

TEST_CASE("End-to-End No Clause") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("constant c; Select c with c.value = 3",
                 {"3"});

  pipeline.query("constant c; Select c",
                 {"0", "1", "2", "3", "5"});

  pipeline.query("procedure p; Select p",
                 {"Example"});
}

TEST_CASE("End-to-End Follows Clause Test") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("stmt s; Select s such that Follows*(1, s) and Follows*(s, 3)",
                 {"2"});

  pipeline.query("stmt s; assign a; Select <s,a> such that Follows(s, s)",
                 {});

  pipeline.query("assign a1, a2; Select a1 such that Follows(1, 2) pattern a2(_,_)",
                 {"1", "2", "3", "5", "7", "8", "9", "11", "12"});

  pipeline.query("assign a; Select a such that Follows(1, 2) pattern a(\"x\",_)",
                 {"1", "5"});

  pipeline.query("assign a; Select a such that Follows(a, 2) pattern a(\"x\",_)",
                 {"1"});

  pipeline.query("assign a; Select a such that Follows*(a, 4) pattern a(\"x\",_)",
                 {"1"});

  pipeline.query("assign a; Select a such that Follows*(a, 4) pattern a(\"i\",_)",
                 {"3"});

}

TEST_CASE("End-to-End Modifies and Uses Test") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("assign a; variable v; Select a such that Uses(a, v) pattern a(v,_)",
                 {"5", "9", "11"});

  pipeline.query("while w; assign a; variable v; Select w such that Uses(a, v) pattern a(v,\"a\")",
                 {});

  pipeline.query("assign a; Select a such that Uses(a, \"x\")",
                 {"5", "7", "8", "9", "12"});

  pipeline.query("assign a; Select a such that Modifies(a, \"x\")",
                 {"1", "5"});

  pipeline.query("read r; Select r such that Modifies(\"Example\", \"x\")",
                 {"10"});
}

TEST_CASE("End-to-End Assign Pattern Test") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("assign a; Select a pattern a (\"x\", _\"1\"_)",
                 {"5"});

  pipeline.query("assign a; variable v; Select a pattern a (v, _\"1\"_) with v.varName = \"x\"",
                 {"5"});

  pipeline.query("assign a; Select a pattern a (\"y\", _\"x\"_)",
                 {"8"});

  pipeline.query("assign a; Select a pattern a (\"x\", _\"9\"_)",
                 {});

  pipeline.query("assign a; Select a pattern a (\"g\", _)",
                 {});

  pipeline.query("assign a; Select a pattern a (\"g\", _)",
                 {});

  pipeline.query("assign a; Select a pattern a (_, _\"1\"_)",
                 {"5", "7", "11", "12"});

  pipeline.query("assign a; Select a pattern a (_, _\"x\"_)",
                 {"5", "7", "8", "9", "12"});

  pipeline.query("assign a; Select a pattern a (_, _)",
                 {"1", "2", "3", "5", "7", "8", "9", "11", "12"});

  pipeline.query("assign a; variable v; Select a pattern a (v, _)",
                 {"1", "2", "3", "5", "7", "8", "9", "11", "12"});

  pipeline.query("assign a; variable v; Select v pattern a (v, _)",
                 {"x", "z", "i", "y"});

  pipeline.query("assign a; variable v; Select a pattern a (v, _\"1\"_)",
                 {"5", "7", "11", "12"});

  pipeline.query("assign a; variable v; Select v pattern a (v, _\"1\"_)",
                 {"z", "x", "i"});

  pipeline.query("assign a; variable v; Select a pattern a (v, _\"z\"_)",
                 {"8", "9"});

  pipeline.query("assign a; Select a pattern a(_, _\"z + x\"_)",
                 {"8", "9"});

  pipeline.query("assign a; Select a pattern a(_, \"z + x\")",
                 {"8"});
}

TEST_CASE("End-to-End If Pattern Test") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("if ifs; Select ifs pattern ifs (\"x\", _, _)",
                 {"6"});

  pipeline.query("if ifs; variable v; Select ifs pattern ifs (v, _, _) with v.varName = \"x\"",
                 {"6"});

  pipeline.query("if ifs; Select ifs pattern ifs (\"y\", _, _)",
                 {});

  pipeline.query("if ifs; variable v; Select ifs pattern ifs (v, _, _) with v.varName = \"y\"",
                 {});

  pipeline.query("if ifs; variable v; Select <v, ifs> pattern ifs (v, _, _)",
                 {"x 6"});

  pipeline.query("if ifs; Select ifs pattern ifs (_, _, _)",
                 {"6"});
}

TEST_CASE("End-to-End While Pattern Test") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("while while; Select while pattern while (\"i\", _)",
                 {"4"});

  pipeline.query("while while; variable v; Select while pattern while (v, _) with v.varName = \"i\"",
                 {"4"});

  pipeline.query("while while; Select while pattern while (\"x\", _)",
                 {});

  pipeline.query("while while; variable v; Select while pattern while (v, _) with v.varName =\"x\"",
                 {});

  pipeline.query("while while; variable v; Select <v, while> pattern while (v, _)",
                 {"i 4"});

  pipeline.query("while while; Select while pattern while (_, _)",
                 {"4"});

  pipeline.query("while w; Select w pattern w(_,_) such that Parent(w, 5)",
                 {"4"});
}

TEST_CASE("End-to-End Pattern Error") {
  auto pipeline = TestPipelineProvider();

  pipeline.expectSyntaxError("assign a; Select a pattern a(_, _\"(a % * 7))\"_)");

  pipeline.expectSyntaxError("assign a; Select a pattern a(_, _\"((a * 7)\"_)");
}

TEST_CASE("End-to-End Multi-Clause") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("assign a1, a2; variable v; Select v such that Modifies(a1, v) pattern a2(\"a\", \"huehuehue\")",
                 {});
}

TEST_CASE("End-to-End Next Test") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("read r; Select r such that Next(4, 5)",
                 {"10"});

  pipeline.query("read r; Select r such that Next(4, 12)",
                 {"10"});

  pipeline.query("read r; Select r such that Next(4, 6)",
                 {});

  pipeline.query("stmt s; Select s such that Next(4, s)",
                 {"5", "12"});

  pipeline.query("stmt s; Select s such that Next(6, s)",
                 {"7", "8"});

  pipeline.query("stmt s; Select s such that Next(s, 9)",
                 {"7", "8"});

  pipeline.query("stmt s1, s2; Select <s1, s2> such that Next(s1, s2)",
                 {
                     "1 2", "2 3", "3 4", "4 5", "4 12",
                     "5 6", "6 7", "6 8", "7 9", "8 9", "9 10",
                     "10 11", "11 4"});

  pipeline.query("read r; Select r such that Next*(1, 12)",
                 { "10" });

  pipeline.query("read r; Select r such that Next*(5, 4)",
                 { "10" });

  pipeline.query("stmt s; Select s such that Next*(5, s)",
                 { "4", "5", "6", "7", "8", "9", "10", "11", "12" });

  pipeline.query("stmt s; Select s such that Next*(s, 4)",
                 { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" });

  pipeline.query("assign a1, a2; Select <a1, a2> such that Next*(a1, a2)",
                 {
                     "1 2", "1 3", "1 5", "1 7", "1 8", "1 9", "1 11", "1 12",
                     "2 3", "2 5", "2 7", "2 8", "2 9", "2 11", "2 12",
                     "3 5", "3 7", "3 8", "3 9", "3 11", "3 12",
                     "5 5", "5 7", "5 8", "5 9", "5 11", "5 12",
                     "7 5", "7 7", "7 8", "7 9", "7 11", "7 12",
                     "8 5", "8 7", "8 8", "8 9", "8 11", "8 12",
                     "9 5", "9 7", "9 8", "9 9", "9 11", "9 12",
                     "11 5", "11 7", "11 8", "11 9", "11 11", "11 12"
                 });

  pipeline.query("assign a1; Select <a1, a1> such that Next*(a1, a1)",
                 {"5 5", "7 7", "8 8", "9 9", "11 11"});
}

TEST_CASE("With Clause Tests - Cat 1 (static = static)") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("Select BOOLEAN with 1 = 1", {"TRUE"});
  pipeline.query("variable v; Select v with 1 = 1", {"x", "z", "i", "y"});

  pipeline.query("Select BOOLEAN with 1 = 2", {"FALSE"});
  pipeline.query("variable v; Select v with 1 = 2", {});

  pipeline.query("Select BOOLEAN with \"foo\" = \"foo\"", {"TRUE"});
  pipeline.query("variable v; Select v with \"foo\" = \"foo\"", {"x", "z", "i", "y"});
}

TEST_CASE("With Clause Tests - Cat 2 (attrRef = static)") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("stmt s1, s2; Select <s1,s2> such that Follows*(s1,s2) with s1.stmt# = s2.stmt#", {});

  pipeline.query("assign a; Select BOOLEAN with a.stmt# = 4", {"FALSE"});
  pipeline.query("variable v; Select BOOLEAN with v.varName = \"x\"", {"TRUE"});
  pipeline.query("variable v; Select BOOLEAN with v.varName = \"g\"", {"FALSE"});
  pipeline.query("assign a; variable v; Select v.varName with v.varName = \"x\" such that Uses(a, v)", {"x"});

  pipeline.query("assign a; Select BOOLEAN with a.stmt# = 1", {"TRUE"});
  pipeline.query("assign a; Select BOOLEAN with a.stmt# = 4", {"FALSE"});
  pipeline.query("assign a1, a2; Select a2.stmt# with a2.stmt# = 3 such that Follows*(a1,a2)", {"3"});

  pipeline.query("while w; Select BOOLEAN with w.stmt# = 4", {"TRUE"});
  pipeline.query("while w; Select BOOLEAN with w.stmt# = 1", {"FALSE"});
  pipeline.query("assign a1;while a2; Select a2.stmt# with a2.stmt# = 4 such that Follows*(a1,a2)", {"4"});

  pipeline.query("if ifs; Select BOOLEAN with ifs.stmt# = 6", {"TRUE"});
  pipeline.query("if ifs; Select BOOLEAN with ifs.stmt# = 1", {"FALSE"});
  pipeline.query("assign a1;if a2; Select a2.stmt# with a2.stmt# = 6 such that Follows*(a1,a2)", {"6"});

  pipeline.query("constant c; Select BOOLEAN with c.value = 1", {"TRUE"});
  pipeline.query("constant c; Select BOOLEAN with c.value = 99", {"FALSE"});

  pipeline.query("read r; Select BOOLEAN with r.stmt# = 10", {"TRUE"});
  pipeline.query("read r; Select BOOLEAN with r.stmt# = 1", {"FALSE"});
  pipeline.query("assign a1;read r; Select r.stmt# with r.stmt# = 10 such that Follows*(a1,r)", {"10"});
  pipeline.query("assign a1;read r; Select r.stmt# with r.varName = \"x\" such that Follows*(a1,r)", {"10"});

  pipeline.query("read r; Select BOOLEAN with r.varName = \"x\"", {"TRUE"});
  pipeline.query("read r; Select BOOLEAN with r.varName = \"g\"", {"FALSE"});
  pipeline.query("assign a1;read r; Select r.varName with r.varName = \"x\" such that Modifies(r,_)", {"x"});
  pipeline.query("assign a1;read r; Select r.varName with r.stmt# = 10 such that Modifies(r,_)", {"x"});

  pipeline.query("stmt s1, s2; Select s1 with s1.stmt# = s2.stmt#", {"1","2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"});

}

TEST_CASE("Out of Bounds Statement") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("stmt r; Select r such that Follows(99, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Follows(r, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Follows(100, r)",
                 {});

  pipeline.query("stmt r; Select r such that Follows*(99, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Follows*(r, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Follows*(100, r)",
                 {});

  pipeline.query("stmt r; Select r such that Parent(99, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Parent(r, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Parent(100, r)",
                 {});

  pipeline.query("stmt r; Select r such that Parent*(99, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Parent*(r, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Parent*(100, r)",
                 {});

  pipeline.query("stmt r; Select r such that Uses(99, _)",
                 {});
  pipeline.query("stmt r; Select r such that Modifies(99, _)",
                 {});

  pipeline.query("stmt r; Select r such that Next(99, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Next(r, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Next(100, r)",
                 {});
  pipeline.query("stmt r; Select r such that Next*(98, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Next*(r, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Next*(100, r)",
                 {});

  pipeline.query("stmt r; Select r such that Affects(99, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Affects(r, 100)",
                 {});
  pipeline.query("stmt r; Select r such that Affects(100, r)",
                 {});
}

TEST_CASE("End-to-End Attribute Projection Test") {
  auto pipeline = TestPipelineProvider();

  // stmt#
  pipeline.query("assign a; Select a.stmt#", {"1", "2", "3", "5", "7", "8", "9", "11", "12"});
  pipeline.query("if a; Select a.stmt#", {"6"});
  pipeline.query("while a; Select a.stmt#", {"4"});
  pipeline.query("call a; Select a.stmt#", {});
  pipeline.query("print a; Select a.stmt#", {});
  pipeline.query("read a; Select a.stmt#", {"10"});
  pipeline.query("stmt a; Select a.stmt#", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"});

  // procName
  pipeline.query("procedure p; Select p.procName", {"Example"});
  pipeline.query("call p; Select p.procName", {});

  // varName
  pipeline.query("variable v; Select v.varName", {"x", "z", "i", "y"});
  pipeline.query("read v; Select v.varName", {"x"});
  pipeline.query("print v; Select v.varName", {});

  // value
  pipeline.query("constant c; Select c.value", {"2", "3", "5", "1", "0"});
}

TEST_CASE("Override With Clause Test, Non Default") {
  auto pipeline = TestPipelineProvider(SOURCE2);

  pipeline.query("read r; Select r with r.varName = \"x\"",
                 {"2", "3"});

  pipeline.query("read r; Select r with r.varName = \"y\"",
                 {"1", "4"});

  pipeline.query("read r; Select r with r.varName = \"z\"",
                 {});
}

TEST_CASE("Override With Clause Test, Default") {
  auto pipeline = TestPipelineProvider(SOURCE2);

  pipeline.query("read r; Select r with r.stmt# = 2",
                 {"2"});
  pipeline.query("stmt s; Select s with s.stmt# = 2",
                 {"2"});
}

TEST_CASE("With Clause Constant Constraint") {
  auto pipeline = TestPipelineProvider(SOURCE2);

  pipeline.query("Select BOOLEAN with 2 = 3",
                 {"FALSE"});
  pipeline.query("Select BOOLEAN with 2 = 2",
                 {"TRUE"});
  pipeline.query("stmt s; Select s such that Follows(1, s) with 2 = 3",
                 {});
  pipeline.query("stmt s; Select s such that Follows(1, s) with 2 = 2",
                 {"2"});
}

TEST_CASE("With Clause Different Synonyms") {
  auto pipeline = TestPipelineProvider(SOURCE3);

  pipeline.query("print p; call cl; Select p with p.varName = cl.procName",
                 {"2"});
  pipeline.query("print p; procedure pr; Select pr with p.varName = pr.procName",
                 {"p2"});
}

TEST_CASE("End-to-End Affects Test") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("read r; Select r such that Affects(1, 5)",
                 {"10"});
  pipeline.query("read r; Select r such that Affects(2, 5)",
                 {});
  pipeline.query("assign a; Select a such that Affects(1, a)",
                 {"5", "12"});
  pipeline.query("assign a; Select a such that Affects(a, 11)",
                 {"3", "11"});
  pipeline.query("assign a; Select a such that Affects(11, a)",
                 {"9", "11"});
  pipeline.query("assign a1, a2; Select <a1, a2> such that Affects(a1, a2)",
                 {"1 5", "1 12", "2 8", "2 9", "3 9", "3 11",
                  "5 7", "5 8", "5 9", "7 9", "9 8", "9 9", "11 9", "11 11"});
  pipeline.query("assign a1, a2; Select BOOLEAN such that Affects*(1,5)", {"TRUE"});
  pipeline.query("assign a1, a2; Select <a1, a2> such that Affects*(a1, a2)", {
      "1 5", "1 7", "1 8", "1 9", "1 12",
      "2 8", "2 9", "3 8", "3 9", "3 11",
      "5 7", "5 8", "5 9", "7 8", "7 9",
      "9 8", "9 9", "11 8", "11 9", "11 11"
  });
}

TEST_CASE("With Const = Stmt Number Test") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("stmt s; constant c; Select c with s.stmt# = c.value",
                 {"1", "2", "3", "5"});
}

TEST_CASE("Affects Typing Test") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("read r1; Select r1 such that Affects(r1, r1)",
                 {});
  pipeline.query("read r1, r2; Select r2 such that Affects(r1, r2) with r1.stmt# = r2.stmt#",
                 {});
  pipeline.query("stmt r1, r2; Select r2 such that Affects(r1, r2) with r1.stmt# = r2.stmt#",
                 {"9", "11"});
}

TEST_CASE("With Override Test") {
  auto pipeline = TestPipelineProvider();

  pipeline.expectSemanticError("stmt s; Select s with s.value = 2");
  pipeline.expectSyntaxError("stmt s; Select s with s = 2");
  pipeline.expectSemanticError("stmt s; Select s.value");
}

TEST_CASE("Next Test") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("stmt s; Select s such that Next(6, s)",
                 { "7", "8" });
  pipeline.query("stmt s; Select s such that Next(s, 8)",
                 { "6" });
}

TEST_CASE("AffectsT Test") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("Select BOOLEAN such that Affects(1, 9)",
                 { "FALSE" });
  pipeline.query("Select BOOLEAN such that Affects*(1, 9)",
                 { "TRUE" });
}

TEST_CASE("Repeated Override Test") {
  auto pipeline = TestPipelineProvider();

  pipeline.query("stmt s; Select s with s.stmt# = 2 and s.stmt# = 2",
                 {"2"});
  pipeline.query("stmt s1, s2; Select <s1, s2> with s1.stmt# = 2 and s2.stmt# = 2 and s1.stmt# = s2.stmt#",
                 {"2 2"});
  pipeline.query("stmt s; Select s with s.stmt# = 2 and s.stmt# = 2 and s.stmt# = 3",
                 {});
  pipeline.query("variable v; Select v with v.varName=\"x\" and v.varName=\"x\"",
                 {"x"});
  pipeline.query("variable v; Select v with v.varName=\"x\" and v.varName=\"x\" and v.varName=\"y\"",
                 {});
}
