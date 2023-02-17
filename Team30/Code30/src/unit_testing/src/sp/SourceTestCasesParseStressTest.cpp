// Stress Testing to check if parser can parse the following cases successfully
// without error and/or returning null

#include <memory>
#include <vector>

#include "../../../../lib/catch.hpp"
#include "../../../spa/src/pkb/storage/PKB.h"
#include "../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../spa/src/sp/SourceParser.h"
#include "../../../spa/src/sp/extractor/AbstractExtractor.h"
#include "../../../spa/src/sp/extractor/Extractor.h"
#include "../../../spa/src/sp/extractor/TreeWalker.h"
#include "../../../spa/src/sp/extractor/concrete_extractors/EntityExtractor.h"
#include "../../../spa/src/sp/extractor/concrete_extractors/FollowsExtractor.h"
#include "../../../spa/src/sp/extractor/concrete_extractors/ModifiesExtractor.h"
#include "../../../spa/src/sp/extractor/concrete_extractors/ParentExtractor.h"
#include "../../../spa/src/sp/extractor/concrete_extractors/UsesExtractor.h"
#include "SourceTestCases.cpp"

AST executeExtractors(string input) {
  TreeWalker treeWalker;
  PKB* pkb = new PKB();
  PkbWriter* pkbWriter = new PkbWriter(pkb);
  SourceParser parser;
  vector<shared_ptr<Extractor>> extractors;

  shared_ptr<AbstractExtractor> followsExtractor =
      shared_ptr<AbstractExtractor>(new FollowsExtractor(pkbWriter));
  shared_ptr<AbstractExtractor> parentExtractor =
      shared_ptr<AbstractExtractor>(new ParentExtractor(pkbWriter));
  shared_ptr<AbstractExtractor> entityExtractor =
      shared_ptr<AbstractExtractor>(new EntityExtractor(pkbWriter));
  shared_ptr<AbstractExtractor> usesExtractor =
      shared_ptr<AbstractExtractor>(new UsesExtractor(pkbWriter));
  shared_ptr<AbstractExtractor> modifiesExtractor =
      shared_ptr<AbstractExtractor>(new ModifiesExtractor(pkbWriter));

  extractors.push_back(followsExtractor);
  extractors.push_back(parentExtractor);
  extractors.push_back(entityExtractor);
  extractors.push_back(usesExtractor);
  extractors.push_back(modifiesExtractor);

  AST ast = parser.parseSource(input);
  treeWalker.walkAST(ast, extractors);

  return ast;
}

TEST_CASE("Simple programs for grammar testing") {
  SourceTestCases testcase = SourceTestCases();

  AST ast = executeExtractors(testcase.EMPTY);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.SIMPLE_PROGRAM);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.ONE_LINE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.SAME_NAMES);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.CASE_SENSITIVE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.LONG_NAMES);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.WHITESPACE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.TEST_ASSIGN);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.ASSIGN_TO_SELF);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.TEST_BASIC_PROCEDURE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.TEST_OPERATORS);

  REQUIRE(ast.getRoot() != nullptr);

  /*ast = *new AST(NULL);

  ast = executeExtractors(testcase.CALL_PROCEDURES);

  REQUIRE(ast.getRoot() != nullptr);*/

  /* ast = *new AST(NULL);

    ast = executeExtractors(testcase.CALL_CASE_SENSITIVE);

    REQUIRE(ast.getRoot() != nullptr);*/
}

TEST_CASE("Programs for testing simple if/else cases") {
  SourceTestCases testcase = SourceTestCases();

  AST ast = executeExtractors(testcase.SIMPLE_IF);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.ONE_LINE_IF_STMT);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.SIMPLE_IF_ELSE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.EMPTY_IF);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.TEST_CONDITIONAL);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.TEST_RELATIONAL);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.CHAINED_IF);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.MULTIPLE_IF_ELSE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.CHAINED_IF_ELSE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.MULTI_CHAINED_IF_ELSE);

  REQUIRE(ast.getRoot() != nullptr);

  /*ast = *new AST(NULL);

  ast = executeExtractors(testcase.CALL_IN_IF);

  REQUIRE(ast.getRoot() != nullptr);*/
}

TEST_CASE("Programs testing simple while cases") {
  SourceTestCases testcase = SourceTestCases();

  AST ast = executeExtractors(testcase.SIMPLE_WHILE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.WHILE_WOUT_WHITESPACE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.MULTIPLE_WHILE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.EMPTY_WHILE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.WHILE_INBETWEEN_STATEMENTS);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.CHAINED_WHILE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.MULTI_CHAINED_WHILE);

  REQUIRE(ast.getRoot() != nullptr);

  /*ast = *new AST(NULL);

  ast = executeExtractors(testcase.CALL_IN_WHILE);

  REQUIRE(ast.getRoot() != nullptr);*/

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.IF_IN_WHILE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.WHILE_IN_IF);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.WHILE_IN_IF_ELSE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.MULTIPLE_WHILE_IN_IF_ELSE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.WHILE_CHAIN_IN_IF);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.WHILE_CHAIN_IN_ELSE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.WHILE_CHAIN_IN_IF_ELSE);

  REQUIRE(ast.getRoot() != nullptr);

  /*ast = *new AST(NULL);

  ast = executeExtractors(testcase.IF_ELSE_SINGLE_CALL_IN_WHILE);

  REQUIRE(ast.getRoot() != nullptr);*/

  /*ast = *new AST(NULL);

  ast = executeExtractors(testcase.MULTIPLE_CALLS_IN_WHILE);

  REQUIRE(ast.getRoot() != nullptr);*/

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.CHAINED_WHILE_WITH_IF_IN_BODY);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.MULTIPLE_IF_ELSE_WITH_WHILE_CHAIN);

  REQUIRE(ast.getRoot() != nullptr);
}

TEST_CASE("Complex Programs") {
  SourceTestCases testcase = SourceTestCases();

  AST ast = executeExtractors(testcase.COMPLEX_PROGRAM_ONE);

  REQUIRE(ast.getRoot() != nullptr);

  ast = *new AST(NULL);

  ast = executeExtractors(testcase.COMPLEX_PROGRAM_TWO);

  REQUIRE(ast.getRoot() != nullptr);

  /*ast = *new AST(NULL);

  ast = executeExtractors(testcase.COMPLEX_PROGRAM_THREE);

  REQUIRE(ast.getRoot() != nullptr);*/
}
