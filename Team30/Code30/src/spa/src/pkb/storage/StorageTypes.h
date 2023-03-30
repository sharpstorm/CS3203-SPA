#pragma once

#include <memory>

#include "FollowsTableManager.h"
#include "IndexTableManager.h"
#include "ParentTTableManager.h"
#include "RelationTableManager.h"
#include "StatementStorage.h"
#include "TransitiveRelationTableManager.h"
#include "common/Types.h"
#include "common/cfg/CFG.h"
#include "common/pattern/PatternTrie.h"
#include "pkb/storage/tables/ContiguousSetTable.h"
#include "pkb/storage/tables/ContiguousTable.h"
#include "tables/HashKeySetTable.h"
#include "tables/HashKeyTable.h"

using VariableStorage = IndexTableManager<EntityValue>;
using ConstantStorage = IndexTableManager<EntityValue>;
using VarTable = ContiguousTable<EntityValue>;
using VarRevTable = HashKeyTable<EntityValue, EntityIdx>;
using ConstTable = ContiguousTable<EntityValue>;
using ConstRevTable = HashKeyTable<EntityValue, EntityIdx>;

// stmtNum -> procedure
using ProcedureStmtTable = ContiguousTable<EntityValue>;
using ProcedureValues = EntitySet;
using StmtTable = ContiguousTable<StmtType>;
using StmtRevTable = HashKeySetTable<StmtType, StmtValue>;

using CallDeclarationTable = HashKeyTable<StmtValue, EntityValue>;

using FollowsStorage = FollowsTableManager;
using FollowsTStorage = RelationTableManager<StmtValue, StmtValue>;
using FollowsTable = IntSetTable<StmtValue>;
using FollowsRevTable = IntSetTable<StmtValue>;

using ParentStorage = RelationTableManager<StmtValue, StmtValue>;
using ParentTStorage = ParentTTableManager;
using ParentTable = IntSetTable<StmtValue>;
using ParentRevTable = IntSetTable<StmtValue>;
using ParentTTable = IntTable<StmtValue>;
using ParentTRevTable = IntSetTable<StmtValue>;

using ModifiesStorage = RelationTableManager<StmtValue, EntityValue>;
using ModifiesTable = HashKeySetTable<StmtValue, EntityValue>;
using ModifiesRevTable = HashKeySetTable<EntityValue, StmtValue>;

using ModifiesPStorage = RelationTableManager<EntityValue, EntityValue>;
using ModifiesPTable = HashKeySetTable<EntityValue, EntityValue>;
using ModifiesPRevTable = HashKeySetTable<EntityValue, EntityValue>;

using UsesStorage = RelationTableManager<StmtValue, EntityValue>;
using UsesTable = HashKeySetTable<StmtValue, EntityValue>;
using UsesRevTable = HashKeySetTable<EntityValue, StmtValue>;

using UsesPStorage = RelationTableManager<EntityValue, EntityValue>;
using UsesPTable = HashKeySetTable<EntityValue, EntityValue>;
using UsesPRevTable = HashKeySetTable<EntityValue, EntityValue>;

using CallsStorage = RelationTableManager<EntityValue, EntityValue>;
using CallsTStorage = TransitiveRelationTableManager<EntityValue>;
using CallsTable = HashKeySetTable<EntityValue, EntityValue>;
using CallsRevTable = HashKeySetTable<EntityValue, EntityValue>;

using IfPatternStorage = RelationTableManager<StmtValue, EntityValue>;
using IfPatternTable = HashKeySetTable<StmtValue, EntityValue>;
using IfPatternRevTable = HashKeySetTable<EntityValue, StmtValue>;

using WhilePatternStorage = RelationTableManager<StmtValue, EntityValue>;
using WhilePatternTable = HashKeySetTable<StmtValue, EntityValue>;
using WhilePatternRevTable = HashKeySetTable<EntityValue, StmtValue>;

using AssignStorage = HashKeyTable<StmtValue, PatternTrieSPtr>;
using CFGStorage = HashKeyTable<EntityValue, CFGSPtr>;
