#pragma once

#include <vector>

using std::vector;

class BitField {
 private:
  vector<uint32_t> data;  // Platform-Independent Width

 public:
  explicit BitField(int capacity);
  void set(int bit);
  void unset(int bit);
  bool isSet(int bit);
};
