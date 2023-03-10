// Stress Testing to check if parser can parse the following cases successfully
// without error and/or returning null

#include <memory>
#include <vector>

#include "../../../../lib/catch.hpp"
#include "sp/ast/AST.h"
#include "../../../spa/src/pkb/storage/PKB.h"
#include "../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../spa/src/sp/SourceParser.h"
#include "../../../spa/src/sp/extractor/AbstractExtractor.h"
#include "sp/extractor/IExtractor.h"
#include "../../../spa/src/sp/extractor/TreeWalker.h"
#include "../../../spa/src/sp/extractor/concrete_extractors/EntityExtractor.h"
#include "../../../spa/src/sp/extractor/concrete_extractors/FollowsExtractor.h"
#include "../../../spa/src/sp/extractor/concrete_extractors/ModifiesExtractor.h"
#include "../../../spa/src/sp/extractor/concrete_extractors/ParentExtractor.h"
#include "../../../spa/src/sp/extractor/concrete_extractors/UsesExtractor.h"
#include "SourceTestCases.cpp"
#include "sp/errors/SPError.h"

using std::make_unique;

void executeExtractors(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  SourceParser parser;
  vector<IExtractor*> extractors;

  auto followsExtractor = make_unique<FollowsExtractor>(&pkbWriter);
  auto parentExtractor = make_unique<ParentExtractor>(&pkbWriter);
  auto entityExtractor = make_unique<EntityExtractor>(&pkbWriter);
  auto usesExtractor = make_unique<UsesExtractor>(&pkbWriter);
  auto modifiesExtractor = make_unique<ModifiesExtractor>(&pkbWriter);

  extractors.push_back(followsExtractor.get());
  extractors.push_back(parentExtractor.get());
  extractors.push_back(entityExtractor.get());
  extractors.push_back(usesExtractor.get());
  extractors.push_back(modifiesExtractor.get());

  AST ast = parser.parseSource(input);
  treeWalker.walkAST(ast, &extractors);
}

TEST_CASE("Simple programs for grammar testing") {
  SourceTestCases testcase = SourceTestCases();

  REQUIRE_THROWS_AS(executeExtractors(testcase.EMPTY), SPError);

  REQUIRE_NOTHROW(executeExtractors(testcase.SIMPLE_PROGRAM));

  REQUIRE_NOTHROW(executeExtractors(testcase.ONE_LINE));

  REQUIRE_NOTHROW(executeExtractors(testcase.SAME_NAMES));

  REQUIRE_NOTHROW(executeExtractors(testcase.CASE_SENSITIVE));

  REQUIRE_NOTHROW(executeExtractors(testcase.LONG_NAMES));

  REQUIRE_NOTHROW(executeExtractors(testcase.WHITESPACE));

  REQUIRE_NOTHROW(executeExtractors(testcase.TEST_ASSIGN));

  REQUIRE_NOTHROW(executeExtractors(testcase.ASSIGN_TO_SELF));

  REQUIRE_NOTHROW(executeExtractors(testcase.TEST_BASIC_PROCEDURE));

  REQUIRE_NOTHROW(executeExtractors(testcase.TEST_OPERATORS));

  REQUIRE_NOTHROW(executeExtractors(testcase.CALL_PROCEDURES));

  REQUIRE_NOTHROW(executeExtractors(testcase.CALL_CASE_SENSITIVE));

}

TEST_CASE("Programs for testing simple if/else cases") {
  SourceTestCases testcase = SourceTestCases();

  REQUIRE_NOTHROW(executeExtractors(testcase.SIMPLE_IF));

  REQUIRE_NOTHROW(executeExtractors(testcase.ONE_LINE_IF_STMT));

  REQUIRE_NOTHROW(executeExtractors(testcase.SIMPLE_IF_ELSE));

  REQUIRE_THROWS_AS(executeExtractors(testcase.EMPTY_IF), SPError);

  REQUIRE_NOTHROW(executeExtractors(testcase.TEST_CONDITIONAL));

  REQUIRE_NOTHROW(executeExtractors(testcase.TEST_RELATIONAL));

  REQUIRE_NOTHROW(executeExtractors(testcase.CHAINED_IF));

  REQUIRE_NOTHROW(executeExtractors(testcase.MULTIPLE_IF_ELSE));

  REQUIRE_NOTHROW(executeExtractors(testcase.CHAINED_IF_ELSE));

  REQUIRE_NOTHROW(executeExtractors(testcase.MULTI_CHAINED_IF_ELSE));

  REQUIRE_NOTHROW(executeExtractors(testcase.CALL_IN_IF));
}

TEST_CASE("Programs testing simple while cases") {
  SourceTestCases testcase = SourceTestCases();

  REQUIRE_NOTHROW(executeExtractors(testcase.SIMPLE_WHILE));

  REQUIRE_NOTHROW(executeExtractors(testcase.WHILE_WOUT_WHITESPACE));

  REQUIRE_NOTHROW(executeExtractors(testcase.MULTIPLE_WHILE));

  REQUIRE_THROWS_AS(executeExtractors(testcase.EMPTY_WHILE), SPError);

  REQUIRE_NOTHROW(executeExtractors(testcase.WHILE_INBETWEEN_STATEMENTS));

  REQUIRE_NOTHROW(executeExtractors(testcase.CHAINED_WHILE));

  REQUIRE_NOTHROW(executeExtractors(testcase.MULTI_CHAINED_WHILE));

  REQUIRE_NOTHROW(executeExtractors(testcase.CALL_IN_WHILE));

  REQUIRE_NOTHROW(executeExtractors(testcase.IF_IN_WHILE));

  REQUIRE_NOTHROW(executeExtractors(testcase.WHILE_IN_IF));

  REQUIRE_NOTHROW(executeExtractors(testcase.WHILE_IN_IF_ELSE));

  REQUIRE_NOTHROW(executeExtractors(testcase.MULTIPLE_WHILE_IN_IF_ELSE));

  REQUIRE_NOTHROW(executeExtractors(testcase.WHILE_CHAIN_IN_IF));

  REQUIRE_NOTHROW(executeExtractors(testcase.WHILE_CHAIN_IN_ELSE));

  REQUIRE_NOTHROW(executeExtractors(testcase.WHILE_CHAIN_IN_IF_ELSE));

  REQUIRE_NOTHROW(executeExtractors(testcase.IF_ELSE_SINGLE_CALL_IN_WHILE));

  REQUIRE_NOTHROW(executeExtractors(testcase.MULTIPLE_CALLS_IN_WHILE));

  REQUIRE_NOTHROW(executeExtractors(testcase.CHAINED_WHILE_WITH_IF_IN_BODY));

  REQUIRE_NOTHROW(
      executeExtractors(testcase.MULTIPLE_IF_ELSE_WITH_WHILE_CHAIN));
}

TEST_CASE("Complex Programs") {
  SourceTestCases testcase = SourceTestCases();

  REQUIRE_NOTHROW(executeExtractors(testcase.COMPLEX_PROGRAM_ONE));

  REQUIRE_NOTHROW(executeExtractors(testcase.COMPLEX_PROGRAM_TWO));

  REQUIRE_NOTHROW(executeExtractors(testcase.COMPLEX_PROGRAM_THREE));
}
