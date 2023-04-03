#pragma once

#include "PQLQuerySynonym.h"

class PQLQuerySynonymProxy {
 private:
  PQLQuerySynonym** syn;

 public:
  explicit PQLQuerySynonymProxy(PQLQuerySynonym** syn);
  void updatePointer(PQLQuerySynonym* newSyn);
  PQLQuerySynonym* get() const;

  PQLQuerySynonym* operator->() const;
  PQLQuerySynonym& operator*() const;
  bool operator==(const PQLQuerySynonymProxy& other) const;
  bool operator!=(const PQLQuerySynonymProxy& other) const;
};
