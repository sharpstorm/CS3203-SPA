#include <memory>
#include "PostProcessWriter.h"
#include "postProcessors/ModifiesUsesPostProcessor.h"

using std::make_unique;
PostProcessWriter::PostProcessWriter(PKB *pkb) {
  addProcessor(make_unique<ModifiesUsesPostProcessor>(pkb).get());
}

void PostProcessWriter::addProcessor(IPostProcessor *p) {
  processors.push_back(p);
};

void PostProcessWriter::runPostProcessor() {
  for (auto itr = processors.begin(); itr != processors.end(); itr++) {
    (*itr)->process();
  }
}
