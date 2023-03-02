#include "SynonymResultTable.h"

SynonymResultTable::SynonymResultTable(PQLQuerySynonymList *mapping,
                                       bool booleanResult) :
    synonymMapping(mapping), booleanResult(booleanResult) { }

SynonymResultTable::SynonymResultTable(PQLQuerySynonymList *mapping) :
    synonymMapping(mapping), booleanResult(false) {}

SynonymResultTable::SynonymResultTable(bool booleanResult) :
    booleanResult(booleanResult), synonymMapping(new PQLQuerySynonymList()) {}

void SynonymResultTable::extractSynonyms(PQLQueryResult *result) {
  // Go through all the columns in targetSyn
  unordered_map<PQLSynonymName, QueryResultItemSet> groupMap;
  for (auto it = synonymMapping->begin(); it != synonymMapping->end(); ++it) {
    ResultTableCol col = result->getSynonymCol(it->getName());

    if (col == PQLQueryResult::NO_COL) {
      continue;
    }

    // If the targetSyn is in result, extract the column values
    QueryResultItemSet valueSet;
    for (int i=0; i < result->getRowCount(); i++) {
      QueryResultTableRow* row = result->getTableRowAt(i);
      QueryResultItem resultItem = *row->at(col);
      valueSet.insert(resultItem);
    }

    synonyms.push_back(*it);
    groupMap.insert({it->getName(), valueSet});
  }
  groupResults.push_back(groupMap);
}

bool SynonymResultTable::getBooleanResult() {
  return booleanResult;
}
bool SynonymResultTable::hasTargetSynonyms() {
  return !synonymMapping->empty();
}

