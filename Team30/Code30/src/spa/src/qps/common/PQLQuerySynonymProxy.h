#pragma once

#include "PQLQuerySynonym.h"

class PQLQuerySynonymProxy {
 private:
  PQLQuerySynonym** syn;

 public:
  explicit PQLQuerySynonymProxy(PQLQuerySynonym** syn);
  void updatePointer(PQLQuerySynonym* newSyn);
  PQLQuerySynonym* get();

  PQLQuerySynonym* operator->();
  PQLQuerySynonym& operator*();
  bool operator==(const PQLQuerySynonymProxy& other) const;
  bool operator!=(const PQLQuerySynonymProxy& other) const;
};
