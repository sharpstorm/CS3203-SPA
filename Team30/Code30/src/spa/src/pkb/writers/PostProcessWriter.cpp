#include "PostProcessWriter.h"

#include <memory>

#include "pkb/writers/postProcessors/CallsTPostProcessor.h"
#include "pkb/writers/postProcessors/CyclicProceduresValidator.h"
#include "pkb/writers/postProcessors/NonExistentProceduresValidator.h"
#include "pkb/writers/postProcessors/ParentTPostProcessor.h"
#include "postProcessors/ModifiesUsesPostProcessor.h"

using std::make_unique, std::unique_ptr, std::move;
PostProcessWriter::PostProcessWriter(PKB *pkb)
    : nonExistentProcedureValidator(
          make_unique<NonExistentProceduresValidator>(pkb)),
      cyclicProceduresValidator(make_unique<CyclicProceduresValidator>(pkb)),
      callsTPostProcessor(make_unique<CallsTPostProcessor>(pkb)),
      parentTPostProcessor(make_unique<ParentTPostProcessor>(pkb)),
      modifiesUsesPostProcessor(make_unique<ModifiesUsesPostProcessor>(pkb)) {}

void PostProcessWriter::runPostProcessor() {
  nonExistentProcedureValidator->validate();
  callsTPostProcessor->process();
  cyclicProceduresValidator->validate();
  parentTPostProcessor->process();
  modifiesUsesPostProcessor->process();
}
