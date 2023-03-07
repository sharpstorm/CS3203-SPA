#pragma once

class IPostValidator {
 public:
  virtual ~IPostValidator() {}

  virtual void validate() = 0;
};
