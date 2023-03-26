#include "catch.hpp"

#include <memory>
#include <unordered_set>
#include "pkb/writers/PkbWriter.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "sp/SpDriver.h"
#include "TestUtils.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/QueryDriver.h"

using std::make_unique, std::make_shared, std::unordered_set, std::to_string;

TEST_CASE("Multiple Procedures End-to-End") {
  auto pkb = make_unique<PKB>();
  auto pkbWriter = make_unique<PkbWriter>(pkb.get());
  auto pkbQH = make_unique<PkbQueryHandler>(pkb.get());

  SpDriver spDriver;
  unique_ptr<IQPS> qps = make_unique<QueryDriver>(pkbQH.get(), &spDriver);
  string simpleSrc =  "procedure Bumblebee {\n"
                      "read x; \n"
                      "call Megatron; \n"
                      "z = p - y;\n"
                      "if (q!=1) then {\n"
                      "    z = x + 24;\n"
                      "    call Ironhide;}\n"
                      "else {\n"
                      "    while (z>0) {\n"
                      "        y = x * q - 5;\n"
                      "        z = z - 1;}\n"
                      "    y = x + y * z + p * q;\n"
                      "    call Ironhide;}\n"
                      "i = x + j + z;}\n"
                      "procedure Megatron {\n"
                      "p = x * 10;\n"
                      "q = y + y * 10;\n"
                      "while (t==0) {\n"
                      "    a = 5 * p - 5;\n"
                      "    print y;}\n"
                      "q = p * 5;}\n"
                      "procedure Ironhide {\n"
                      "i = x * 10;\n"
                      "j = x + y * 10;\n"
                      "if (i>j) then {\n"
                      "    read x;\n"
                      "    call Barricade;}\n"
                      "else {\n"
                      "    y = i + x + y * z;}}\n"
                      "procedure Barricade {\n"
                      "    q = i * x * j + y + p * q;\n"
                      "    print q;}"
                      ;
  spDriver.parseSource(simpleSrc, pkbWriter.get());

  string query;
  unordered_set<string> expectedRes;

  // Print cat 2
  query = "print p; Select BOOLEAN with p.stmt# = 1";
  expectedRes = unordered_set<string>({"FALSE"});
  launchQuery(qps.get(), query, expectedRes);

  query = "print p; Select BOOLEAN with p.stmt# = 17";
  expectedRes = unordered_set<string>({"TRUE"});
  launchQuery(qps.get(), query, expectedRes);

  query = "print p; Select BOOLEAN with p.varName = \"q\"";
  expectedRes = unordered_set<string>({"TRUE"});
  launchQuery(qps.get(), query, expectedRes);

  query = "print p; Select BOOLEAN with p.varName = \"asdf\"";
  expectedRes = unordered_set<string>({"FALSE"});
  launchQuery(qps.get(), query, expectedRes);

  query = "print p; Select p.stmt# with p.stmt# = 17 such that Uses(p, _)";
  expectedRes = unordered_set<string>({"17"});
  launchQuery(qps.get(), query, expectedRes);

  query = "print p; Select p.varName with p.varName = \"q\" such that Uses(p, _)";
  expectedRes = unordered_set<string>({"q"});
  launchQuery(qps.get(), query, expectedRes);

  query = "print p; Select p.stmt# with p.varName = \"q\" such that Uses(p, _)";
  expectedRes = unordered_set<string>({"26"});
  launchQuery(qps.get(), query, expectedRes);

  query = "print p; Select p.varName with p.stmt# = 17 such that Uses(p, _)";
  expectedRes = unordered_set<string>({"y"});
  launchQuery(qps.get(), query, expectedRes);

  // Call cat 2
  query = "call c; Select BOOLEAN with c.procName = \"Optimus\"";
  expectedRes = unordered_set<string>({"FALSE"});
  launchQuery(qps.get(), query, expectedRes);

  query = "call c; Select BOOLEAN with c.procName = \"Megatron\"";
  expectedRes = unordered_set<string>({"TRUE"});
  launchQuery(qps.get(), query, expectedRes);

  query = "call c; Select BOOLEAN with c.stmt# = 1";
  expectedRes = unordered_set<string>({"FALSE"});
  launchQuery(qps.get(), query, expectedRes);

  query = "call c; Select BOOLEAN with c.stmt# = 2";
  expectedRes = unordered_set<string>({"TRUE"});
  launchQuery(qps.get(), query, expectedRes);

  query = "call c; Select c.stmt# with c.stmt# = 2 such that Uses(c, _)";
  expectedRes = unordered_set<string>({"2"});
  launchQuery(qps.get(), query, expectedRes);

  query = "call c; Select c.procName with c.procName = \"Ironhide\" such that Uses(c, _)";
  expectedRes = unordered_set<string>({"Ironhide"});
  launchQuery(qps.get(), query, expectedRes);

  query = "call c; Select c.stmt# with c.procName = \"Ironhide\" such that Uses(c, _)";
  expectedRes = unordered_set<string>({"6", "11"});
  launchQuery(qps.get(), query, expectedRes);

  query = "call c; Select c.procName with c.stmt# = 2 such that Uses(c, _)";
  expectedRes = unordered_set<string>({"Megatron"});
  launchQuery(qps.get(), query, expectedRes);

  // Procedure cat 2
  query = "procedure p; Select BOOLEAN with p.procName = \"Megatron\"";
  expectedRes = unordered_set<string>({"TRUE"});
  launchQuery(qps.get(), query, expectedRes);

  query = "procedure p; Select BOOLEAN with p.procName = \"Optimus\"";
  expectedRes = unordered_set<string>({"FALSE"});
  launchQuery(qps.get(), query, expectedRes);

  query = "procedure p; Select p.procName with p.procName = \"Ironhide\" such that Calls(p,_)";
  expectedRes = unordered_set<string>({"Ironhide"});
  launchQuery(qps.get(), query, expectedRes);

  // Calls query
  query = "procedure p,q; Select <p,q> such that Calls(p,q)";
  expectedRes = unordered_set<string>({"Bumblebee Megatron", "Bumblebee Ironhide", "Ironhide Barricade"});
  launchQuery(qps.get(), query, expectedRes);

  query = "procedure p,q; Select <p,q> such that Calls*(p,q)";
  expectedRes = unordered_set<string>({"Bumblebee Megatron", "Bumblebee Ironhide", "Ironhide Barricade", "Bumblebee Barricade"});
  launchQuery(qps.get(), query, expectedRes);

  query = "procedure p; Select p such that Calls(_,_)";
  expectedRes = unordered_set<string>({"Bumblebee", "Ironhide", "Barricade", "Megatron"});
  launchQuery(qps.get(), query, expectedRes);

  query = "procedure p; Select p such that Calls*(_,_)";
  launchQuery(qps.get(), query, expectedRes);

  query = "procedure q; Select q such that Calls(_,q)";
  expectedRes = unordered_set<string>({"Megatron", "Ironhide", "Barricade"});
  launchQuery(qps.get(), query, expectedRes);

  query = "procedure p,q; Select p such that Calls(p,q)";
  expectedRes = unordered_set<string>({"Bumblebee", "Ironhide"});
  launchQuery(qps.get(), query, expectedRes);

  query = "procedure p; Select BOOLEAN such that Calls(_,_)";
  expectedRes = unordered_set<string>({"TRUE"});
  launchQuery(qps.get(), query, expectedRes);

  query = "procedure p; Select p such that Calls(_,_)";
  expectedRes = unordered_set<string>({"Bumblebee", "Megatron", "Ironhide", "Barricade"});
  launchQuery(qps.get(), query, expectedRes);

  query = "procedure p; Select p such that Calls(_,p)";
  expectedRes = unordered_set<string>({"Megatron", "Ironhide", "Barricade"});
  launchQuery(qps.get(), query, expectedRes);

  query = "procedure p; Select p such that Calls(p,_)";
  expectedRes = unordered_set<string>({"Bumblebee", "Ironhide"});
  launchQuery(qps.get(), query, expectedRes);


  query = R"(procedure p; Select BOOLEAN such that Calls("Ironhide", "Barricade"))";
  expectedRes = unordered_set<string>({"TRUE"});
  launchQuery(qps.get(), query, expectedRes);

  query = R"(procedure p; Select p such that Calls("Ironhide", "Barricade"))";
  expectedRes = unordered_set<string>({"Bumblebee", "Megatron", "Ironhide", "Barricade"});
  launchQuery(qps.get(), query, expectedRes);

  query = R"(procedure p; Select BOOLEAN such that Calls("Barricade", "Ironhide"))";
  expectedRes = unordered_set<string>({"FALSE"});
  launchQuery(qps.get(), query, expectedRes);

  query = R"(procedure p; Select p such that Calls("Barricade", "Ironhide"))";
  expectedRes = unordered_set<string>({});
  launchQuery(qps.get(), query, expectedRes);
}
