#pragma once

#include <vector>
#include <cstdint>

using std::vector;

class BitField {
 private:
  vector<uint32_t> data;  // Platform-Independent Width
  int capacity;

 public:
  BitField();
  explicit BitField(int capacity);
  void set(int bit);
  void unset(int bit);
  bool isSet(int bit) const;

  bool empty();
  BitField differenceWith(const BitField &other);
  BitField unionWith(const BitField &other);
  bool contains(const BitField &other);
};
