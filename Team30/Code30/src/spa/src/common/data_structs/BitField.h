#pragma once

#include <vector>
#include <cstdint>

using std::vector;

class BitField {
 private:
  static const int UNIT_SIZE = 32;
  vector<uint32_t> data;  // Platform-Independent Width
  int capacity;

 public:
  BitField();
  explicit BitField(const int &capacity);
  void set(int bit);
  void unset(int bit);
  bool isSet(int bit) const;

  bool empty();
  BitField differenceWith(const BitField &other);
  BitField projectOnto(const BitField &other);
  BitField unionWith(const BitField &other);
  bool contains(const BitField &other);
};
