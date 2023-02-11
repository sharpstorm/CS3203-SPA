#pragma once

class IParentWriter {
 public:
  virtual ~IParentWriter() {}
  virtual void addParent(int arg1, int arg2) = 0;
};
