#pragma once

class IPostProcessor {
 public:
  virtual ~IPostProcessor() {}

  virtual void process() = 0;
};
