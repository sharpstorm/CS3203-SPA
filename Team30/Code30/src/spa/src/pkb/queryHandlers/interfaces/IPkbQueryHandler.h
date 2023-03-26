#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "common/pattern/PatternTrie.h"

using std::string, std::unordered_set, std::vector;
using std::unique_ptr;

class IPkbQueryHandler {
 public:
  virtual ~IPkbQueryHandler() {}
  virtual QueryResultPtr<int, int> queryFollows(StmtRef,
                                                         StmtRef) const = 0;
  virtual QueryResultPtr<int, int> queryFollowsStar(StmtRef,
                                                             StmtRef) const = 0;
  virtual QueryResultPtr<int, int> queryParent(StmtRef,
                                                        StmtRef) const = 0;
  virtual QueryResultPtr<int, int> queryParentStar(StmtRef,
                                                            StmtRef) const = 0;
  virtual QueryResultPtr<int, string> queryUses(StmtRef,
                                                         EntityRef) const = 0;
  virtual QueryResultPtr<string, string> queryUses(
      EntityRef, EntityRef) const = 0;
  virtual QueryResultPtr<int, string> queryModifies(
      StmtRef, EntityRef) const = 0;
  virtual QueryResultPtr<string, string> queryModifies(
      EntityRef, EntityRef) const = 0;
  virtual QueryResultPtr<int, PatternTrie *> queryAssigns(
      StmtRef) const = 0;
  virtual QueryResultPtr<string, string> queryCalls(
      EntityRef, EntityRef) const = 0;
  virtual QueryResultPtr<string, string> queryCallsStar(
      EntityRef, EntityRef) const = 0;
  virtual QueryResultPtr<int, string> queryIfPattern(
      StmtRef, EntityRef) const = 0;
  virtual QueryResultPtr<int, string> queryWhilePattern(
      StmtRef, EntityRef) const = 0;
  virtual unordered_set<string> getSymbolsOfType(EntityType) const = 0;
  virtual unordered_set<int> getStatementsOfType(StmtType) const = 0;
  virtual StmtType getStatementType(int) const = 0;
  virtual string getVariableByIndex(int) const = 0;
  virtual string getConstantByIndex(int) const = 0;
  virtual unordered_set<int> getIndexOfVariable(string) const = 0;
  virtual unordered_set<int> getIndexOfConstant(string) const = 0;
  virtual vector<CFG *> queryCFGs(StmtRef) const = 0;
  virtual bool isStatementOfType(StmtType, int) const = 0;
  virtual bool isSymbolOfType(EntityType, string) const = 0;
  virtual string getCalledDeclaration(int) const = 0;
  virtual string getReadDeclarations(int) const = 0;
  virtual string getPrintDeclarations(int) const = 0;
};
