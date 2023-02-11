#pragma once

class IFollowsWriter {
 public:
  virtual ~IFollowsWriter() {};
  virtual void addFollows(int arg1, int arg2) = 0;
};
