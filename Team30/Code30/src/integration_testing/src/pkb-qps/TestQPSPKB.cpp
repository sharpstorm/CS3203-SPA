#include "catch.hpp"

#include <memory>
#include <unordered_set>
#include "qps/QPSFacade.h"
#include "pkb/writers/PkbWriter.h"
#include "qps/errors/QPSError.h"
#include "../TestUtils.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/AST.h"

using std::make_unique, std::make_shared, std::unordered_set, std::to_string;

/*
 * procedure ANYA {
 *   x = 1;
 *   y = 2;
 *   z = 3;
 *   x = x + 1;
 *   z = y + 2;
 *
 *   while(x == 1) {
 *     while (x == 2) {
 *       while (x == 3) {
 *         read f;
 *       }
 *     }
 *   }
 * }
 *
 */

class SourceParserStub: public ISourceExpressionParser {
 public:
  IASTPtr parseExpression(string expression) override {
    auto root = make_shared<VariableASTNode>("a");
    auto tree = make_unique<AST>(root);
    return tree;
  }
};

unique_ptr<PKB> initPkb() {
  auto pkb = make_unique<PKB>();
  auto pkbWriter = make_unique<PkbWriter>(pkb.get());

  pkbWriter->addProcedure("ANYA", 1, 5);
  pkbWriter->addStatement(1, StmtType::Assign);
  pkbWriter->addStatement(2, StmtType::Assign);
  pkbWriter->addStatement(3, StmtType::Assign);
  pkbWriter->addStatement(4, StmtType::Assign);
  pkbWriter->addStatement(5, StmtType::Assign);
  pkbWriter->addStatement(6, StmtType::While);
  pkbWriter->addStatement(7, StmtType::While);
  pkbWriter->addStatement(8, StmtType::While);
  pkbWriter->addStatement(9, StmtType::Read);

  pkbWriter->addSymbol("x", EntityType::Variable);
  pkbWriter->addSymbol("y", EntityType::Variable);
  pkbWriter->addSymbol("z", EntityType::Variable);

  pkbWriter->addFollows(1, 2);
  pkbWriter->addFollows(2, 3);
  pkbWriter->addFollows(3, 4);
  pkbWriter->addFollows(4, 5);

  pkbWriter->addParent(6, 7);
  pkbWriter->addParent(7, 8);
  pkbWriter->addParent(8, 9);

  pkbWriter->addUses(4, "x", "example");
  pkbWriter->addUses(5, "y", "example");
  pkbWriter->addModifies(1, "x", "example");
  pkbWriter->addModifies(2, "y", "example");
  pkbWriter->addModifies(3, "z", "example");
  pkbWriter->addModifies(4, "x", "example");
  pkbWriter->addModifies(5, "z", "example");

  return pkb;
}

TEST_CASE("Test QP Query Basic Follows") {
  auto pkb = initPkb();
  auto pkbQH_ptr = make_unique<PkbQueryHandler>(pkb.get());
  SourceParserStub sp;
  unique_ptr<IQPS> qps = make_unique<QPSFacade>(pkbQH_ptr.get(), &sp);

  launchQuery(qps.get(), "stmt s1, s2; Select s1 such that Follows(s1, s2)",
              unordered_set<string>{ "1", "2", "3", "4" });
  launchQuery(qps.get(), "stmt s2; Select s2 such that Follows(1, s2)",
              unordered_set<string>{ "2" });
  launchQuery(qps.get(), "stmt s2; Select s2 such that Follows(s2, 4)",
              unordered_set<string>{ "3" });
  launchQuery(qps.get(), "assign s; Select s such that Follows(1, 2)",
              unordered_set<string>{ "1", "2", "3", "4", "5" });
  launchQuery(qps.get(), "stmt s; Select s such that Follows(1, 3)",
              unordered_set<string>{ });

  launchQuery(qps.get(), "assign a; stmt s1; Select s1 such that Follows(a, s1)",
              unordered_set<string>{ "2", "3", "4", "5" });
  launchQuery(qps.get(), "assign a; read r; Select r such that Follows(a, r)",
              unordered_set<string>{ });

  launchQuery(qps.get(), "assign s1, s2; Select s2 such that Follows*(1, s2)",
              unordered_set<string>{ "2", "3", "4", "5" });
  launchQuery(qps.get(), "assign s1, s2; Select s2 such that Follows*(2, s2)",
              unordered_set<string>{ "3", "4", "5" });

  launchQuery(qps.get(), "assign s1, s2; Select s2 such that Follows*(s2, 4)",
              unordered_set<string>{ "1", "2", "3" });
  launchQuery(qps.get(), "assign s1, s2; Select s1 such that Follows*(s2, 4)",
              unordered_set<string>{ "1", "2", "3", "4", "5" });

  launchQuery(qps.get(), "assign s1, s2; Select s1 such that Follows(1, s2)",
              unordered_set<string>{ "1", "2", "3", "4", "5" });
  launchQuery(qps.get(), "assign s1, s2; Select s1 such that Follows(5, s2)",
              unordered_set<string>{ });

  launchQuery(qps.get(), "stmt s; Select s such that Follows(s, s)",unordered_set<string>({ }));
  launchQuery(qps.get(), "assign a; Select a such that Follows(a, a)",unordered_set<string>({ }));

  launchQuery(qps.get(), "assign s1, s2; Select s1",
              unordered_set<string>{ "1", "2", "3", "4", "5" });

  launchQuery(qps.get(), "variable v; Select v",
              unordered_set<string>{ "x", "y", "z" });
}

TEST_CASE("Test QP Query Basic Follows*") {
  auto pkb = initPkb();
  auto pkbQH = make_unique<PkbQueryHandler>(pkb.get());
  SourceParserStub sp;
  unique_ptr<IQPS> qps = make_unique<QPSFacade>(pkbQH.get(), &sp);

  launchQuery(qps.get(), "stmt s1, s2; Select s2 such that Follows*(1, s2)",
              unordered_set<string>{ "2", "3", "4", "5" });
  launchQuery(qps.get(), "stmt s1, s2; Select s1 such that Follows*(s1, 3)",
              unordered_set<string>{ "1", "2" });
}

TEST_CASE("Test QP Query Basic Parent") {
  auto pkb = initPkb();
  auto pkbQH = make_unique<PkbQueryHandler>(pkb.get());
  SourceParserStub sp;
  unique_ptr<IQPS> qps = make_unique<QPSFacade>(pkbQH.get(), &sp);

  launchQuery(qps.get(), "stmt s1, s2; Select s2 such that Parent(s1, s2)",
              unordered_set<string>{ "7", "8", "9" });
  launchQuery(qps.get(), "stmt s1, s2; Select s1 such that Parent(s1, s2)",
              unordered_set<string>{ "6", "7", "8" });
  launchQuery(qps.get(), "stmt s1, s2; Select s2 such that Parent(6, s2)",
              unordered_set<string>{ "7" });
  launchQuery(qps.get(), "stmt s1, s2; Select s1 such that Parent(s1, 8)",
              unordered_set<string>{ "7" });
}

TEST_CASE("Test QP Query Basic Parent*") {
  auto pkb = initPkb();
  auto pkbQH = make_unique<PkbQueryHandler>(pkb.get());
  SourceParserStub sp;
  unique_ptr<IQPS> qps = make_unique<QPSFacade>(pkbQH.get(), &sp);

  launchQuery(qps.get(), "stmt s1, s2; Select s2 such that Parent*(6, s2)",
              unordered_set<string>{ "7", "8", "9" });
  launchQuery(qps.get(), "stmt s1, s2; Select s1 such that Parent*(s1, 9)",
              unordered_set<string>{ "6", "7", "8" });
}

TEST_CASE("Test QP Query Basic Uses") {
  auto pkb = initPkb();
  auto pkbQH = make_unique<PkbQueryHandler>(pkb.get());
  SourceParserStub sp;
  unique_ptr<IQPS> qps = make_unique<QPSFacade>(pkbQH.get(), &sp);

  launchQuery(qps.get(), "variable v; Select v such that Uses(4, \"x\")",
              unordered_set<string>({"x", "y", "z"}));
  launchQuery(qps.get(), "assign a; Select a such that Uses(4, \"x\")",
              unordered_set<string>({ "1", "2", "3", "4", "5" }));
  launchQuery(qps.get(), "assign a; Select a such that Uses(1, \"x\")",
              unordered_set<string>());
  launchQuery(qps.get(), "variable v; Select v such that Uses(1, \"x\")",
              unordered_set<string>());

  launchQuery(qps.get(), "variable v; Select v such that Uses(4, v)",
              unordered_set<string>({"x"}));
  launchQuery(qps.get(), "assign a; Select a such that Uses(a, \"x\")",
              unordered_set<string>({"4"}));

  launchQuery(qps.get(), "variable v; assign a; Select v such that Uses(a, v)",
              unordered_set<string>({"x", "y"}));
  launchQuery(qps.get(), "assign a; variable v; Select a such that Uses(a, v)",
              unordered_set<string>({"4", "5"}));

  launchQuery(qps.get(), "assign a; Select a such that Uses(a, _)",
              unordered_set<string>{"4", "5"});
}

TEST_CASE("Test QP Query Basic Modifies") {
  auto pkb = initPkb();
  auto pkbQH = make_unique<PkbQueryHandler>(pkb.get());
  SourceParserStub sp;
  unique_ptr<IQPS> qps = make_unique<QPSFacade>(pkbQH.get(), &sp);

  launchQuery(qps.get(), "assign a; variable v; Select v such that Modifies(1, \"x\")",
              unordered_set<string>({"x", "y", "z"}));
  launchQuery(qps.get(), "assign a; variable v; Select a such that Modifies(1, \"x\")",
              unordered_set<string>({ "1", "2", "3", "4", "5" }));
  launchQuery(qps.get(), "assign a; variable v; Select v such that Modifies(2, \"x\")",
              unordered_set<string>());
  launchQuery(qps.get(), "assign a; variable v; Select a such that Modifies(2, \"x\")",
              unordered_set<string>());

  launchQuery(qps.get(), "assign a; variable v; Select a such that Modifies(a, \"x\")",
              unordered_set<string>({"1", "4"}));
  launchQuery(qps.get(), "assign a; variable v; Select v such that Modifies(5, v)",
              unordered_set<string>({"z"}));

  launchQuery(qps.get(), "assign a; variable v; Select a such that Modifies(a,v)",
              unordered_set<string>({"1", "2", "3", "4", "5"}));
  launchQuery(qps.get(), "assign a; variable v; Select v such that Modifies(a,v)",
              unordered_set<string>({"x", "y", "z"}));

  launchQuery(qps.get(), "assign a; Select a such that Modifies(a, _)",
              unordered_set<string>({ "1", "2", "3", "4", "5" }));
}
