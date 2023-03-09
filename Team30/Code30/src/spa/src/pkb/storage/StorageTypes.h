#pragma once

#include <memory>
#include <string>

#include "EntityTableManager.h"
#include "RelationTableManager.h"
#include "TransitiveRelationTableManager.h"
#include "PatternTableManager.h"
#include "tables/HashKeyTable.h"
#include "tables/HashKeySetTable.h"
#include "common/pattern/PatternTrie.h"

using std::string;

using StatementStorage = EntityTableManager<int, StmtType>;
using ProcedureStorage = EntityTableManager<int, string>;
using FollowsStorage = TransitiveRelationTableManager<int>;
using ParentStorage = TransitiveRelationTableManager<int>;
using SymbolStorage = HashKeySetTable<EntityType, string>;
using UsesStorage = RelationTableManager<int, string>;
using UsesPStorage = RelationTableManager<string, string>;
using ModifiesStorage = RelationTableManager<int, string>;
using ModifiesPStorage = RelationTableManager<string, string>;
using AssignStorage = HashKeyTable<int, PatternTrieSPtr>;
using CallsStorage = TransitiveRelationTableManager<string>;
using CallStmtStorage = EntityTableManager<int, string>;
using IfPatternStorage = PatternTableManager<int, string>;
using WhilePatternStorage = PatternTableManager<int, string>;
