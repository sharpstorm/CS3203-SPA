#include <memory>
#include <vector>
#include <utility>

#include "qps/common/PQLQueryResult.h"

using std::vector, std::move, std::make_unique, std::unique_ptr;

typedef vector<QueryResultItem> QueryResultItemVector;
typedef unordered_map<string, QueryResultItemVector> ExpectedParams;

class TestQueryResultBuilder {
 public:
  static unique_ptr<PQLQueryResult> buildExpected(const ExpectedParams &expectedParams) {
    auto result = make_unique<PQLQueryResult>();

    vector<vector<QueryResultItemPtr>> toInsert;
    for (auto pair : expectedParams) {
      result->putSynonym(pair.first);

      for (int i = 0; i < pair.second.size(); i++) {
        if (toInsert.size() <= i) {
          toInsert.push_back(vector<QueryResultItemPtr>{});
        }
        toInsert.at(i).push_back(make_unique<QueryResultItem>(pair.second.at(i)));
      }
    }

    for (int i = 0; i < toInsert.size(); i++) {
      result->putTableRow(move(toInsert.at(i)));
    }

    return result;
  }
};
