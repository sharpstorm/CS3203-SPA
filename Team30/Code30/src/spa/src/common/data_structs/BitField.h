#pragma once

#include <vector>
#include <cstdint>
#include <unordered_set>

using std::vector, std::unordered_set;

class BitField {
 private:
  static const int UNIT_SIZE = 32;
  vector<uint32_t> data;  // Platform-Independent Width
  int capacity;

 public:
  BitField();
  explicit BitField(const int &capacity);
  void set(int bit);
  void set(const unordered_set<int> &bits);
  void unset(int bit);
  void unset(const unordered_set<int> &bits);
  bool isSet(int bit) const;

  bool empty();
  BitField differenceWith(const BitField &other);
  BitField projectOnto(const BitField &other);
  BitField unionWith(const BitField &other);
  bool contains(const BitField &other);
};
