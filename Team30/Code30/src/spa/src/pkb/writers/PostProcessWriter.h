#pragma once

#include <memory>
#include <vector>

#include "interfaces/IPostProcessWriter.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/interfaces/IPkbWriter.h"
#include "pkb/writers/postProcessors/IPostProcessor.h"
#include "pkb/writers/postProcessors/IPostValidator.h"

using std::vector, std::unique_ptr;

class PostProcessWriter : public IPostProcessWriter {
 public:
  explicit PostProcessWriter(PKB *);
  void runPostProcessor() override;

 private:
  unique_ptr<IPostValidator> nonExistentProcedureValidator;
  unique_ptr<IPostValidator> cyclicProceduresValidator;
  unique_ptr<IPostProcessor> callsTPostProcessor;
  unique_ptr<IPostProcessor> parentTPostProcessor;
  unique_ptr<IPostProcessor> modifiesUsesPostProcessor;
};
