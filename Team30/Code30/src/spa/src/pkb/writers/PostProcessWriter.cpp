#include <memory>
#include "PostProcessWriter.h"
#include "postProcessors/ModifiesUsesPostProcessor.h"
#include "pkb/writers/postProcessors/NonExistentProceduresValidator.h"
#include "pkb/writers/postProcessors/CyclicProceduresValidator.h"

using std::make_unique, std::unique_ptr, std::move;
PostProcessWriter::PostProcessWriter(PKB *pkb) {
  validators.push_back(make_unique<NonExistentProceduresValidator>(pkb));
  validators.push_back(make_unique<CyclicProceduresValidator>(pkb));

  processors.push_back(make_unique<ModifiesUsesPostProcessor>(pkb));
}

void PostProcessWriter::runPostProcessor() {
  for (auto itr = validators.begin(); itr != validators.end(); itr++) {
    (*itr)->validate();
  }

  for (auto itr = processors.begin(); itr != processors.end(); itr++) {
    (*itr)->process();
  }
}
