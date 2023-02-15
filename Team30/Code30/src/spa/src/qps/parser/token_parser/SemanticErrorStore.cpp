#include "SemanticErrorStore.h"

SemanticErrorStore::SemanticErrorStore(): semanticErrorMsg("") {
}

void SemanticErrorStore::setSemanticError(string error) {
  if (semanticErrorMsg != "") {
    return;
  }

  semanticErrorMsg = error;
}

bool SemanticErrorStore::hasSemanticError() {
  return semanticErrorMsg != "";
}

string SemanticErrorStore::getSemanticError() {
  return semanticErrorMsg;
}
