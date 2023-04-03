#include "PQLQuerySynonymProxy.h"

PQLQuerySynonymProxy::PQLQuerySynonymProxy(PQLQuerySynonym **syn):
    syn(syn) {
}

void PQLQuerySynonymProxy::updatePointer(PQLQuerySynonym *newSyn) {
  *syn = newSyn;
}

PQLQuerySynonym *PQLQuerySynonymProxy::get() const {
  return *syn;
}

PQLQuerySynonym &PQLQuerySynonymProxy::operator*() const {
  return *(*(this->syn));
}

PQLQuerySynonym *PQLQuerySynonymProxy::operator->() const {
  return *(this->syn);
}

bool PQLQuerySynonymProxy::operator==(const PQLQuerySynonymProxy &other) const {
  return syn == other.syn;
}

bool PQLQuerySynonymProxy::operator!=(const PQLQuerySynonymProxy &other) const {
  return syn != other.syn;
}
