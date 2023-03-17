#pragma once

#include <string>
#include <memory>
#include <unordered_set>
#include <unordered_map>

using std::string, std::unique_ptr, std::unordered_set, std::unordered_map;

class QueryResultItem {
 private:
  static const int NO_STMT_REF = -1;
  static const char NO_ENT_REF[];
  int stmtRef;
  string entRef;

 public:
  explicit QueryResultItem(int stmt): stmtRef(stmt), entRef(NO_ENT_REF) {}
  explicit QueryResultItem(string ent): stmtRef(NO_STMT_REF), entRef(ent) {}
  string getEntRef();
  int getStmtRef();
  string project();
  bool operator==(const QueryResultItem& other) const;
  bool operator!=(const QueryResultItem& other) const;

  struct hasher {
    std::size_t operator()(const QueryResultItem& k) const {
      if (k.stmtRef != NO_STMT_REF) {
        return std::hash<int>()(k.stmtRef);
      } else {
        return std::hash<string>()(k.entRef);
      }
    }
  };
};

typedef unique_ptr<QueryResultItem> QueryResultItemPtr;

template<class T>
using QueryResultItemMap = unordered_map<QueryResultItem, T,
                                         QueryResultItem::hasher>;
using QueryResultItemSet = unordered_set<QueryResultItem,
                                         QueryResultItem::hasher>;
