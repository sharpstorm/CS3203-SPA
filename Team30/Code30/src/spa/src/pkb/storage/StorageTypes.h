#pragma once

#include <memory>
#include <string>

#include "EntityTableManager.h"
#include "RelationTableManager.h"
#include "TransitiveRelationTableManager.h"
#include "tables/HashKeyTable.h"
#include "tables/HashKeySetTable.h"
#include "common/pattern/PatternTrie.h"
#include "common/cfg/CFG.h"

using std::string;

using StatementStorage = EntityTableManager<int, StmtType>;
using ProcedureStorage = EntityTableManager<int, string>;
using FollowsStorage = TransitiveRelationTableManager<int>;
using ParentStorage = TransitiveRelationTableManager<int>;
using VariableStorage = EntityTableManager<int, string>;
using ConstantStorage = EntityTableManager<int, string>;
using StatementStorage = EntityTableManager<int, StmtType>;
using ProcedureStorage = EntityTableManager<int, string>;
using UsesStorage = RelationTableManager<int, string>;
using UsesPStorage = RelationTableManager<string, string>;
using ModifiesStorage = RelationTableManager<int, string>;
using ModifiesPStorage = RelationTableManager<string, string>;
using AssignStorage = HashKeyTable<int, PatternTrieSPtr>;
using CallsStorage = TransitiveRelationTableManager<string>;
using CallStmtStorage = EntityTableManager<int, string>;
using IfPatternStorage = RelationTableManager<int, string>;
using WhilePatternStorage = RelationTableManager<int, string>;
using CFGStorage = HashKeyTable<string, CFGSPtr>;
