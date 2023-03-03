#include <memory>
#include "PostProcessWriter.h"
#include "postProcessors/ModifiesUsesPostProcessor.h"

using std::make_unique, std::unique_ptr, std::move;
PostProcessWriter::PostProcessWriter(PKB *pkb) {
  processors.push_back(make_unique<ModifiesUsesPostProcessor>(pkb));
}

void PostProcessWriter::runPostProcessor() {
  for (auto itr = processors.begin(); itr != processors.end(); itr++) {
    (*itr)->process();
  }
}
