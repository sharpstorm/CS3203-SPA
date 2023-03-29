#pragma once

#include <vector>
#include <cstdint>
#include <unordered_set>

using std::vector, std::unordered_set;

class BitField {
 private:
  constexpr static int UNIT_SIZE = 32;
  constexpr static int MASK_8BITS[] =
      {0x00, 0x01, 0x03, 0x07, 0x0F,
       0x1F, 0x3F, 0x7F, 0xFF};
  constexpr static int FULL_MASK_LOWER_32 = 0xFFFFFFFF;
  constexpr static int FULL_MASK_LOWER_24 = 0x00FFFFFF;
  constexpr static int FULL_MASK_LOWER_16 = 0x0000FFFF;
  constexpr static int FULL_MASK_LOWER_8 = 0x000000FF;

  constexpr static int BITS_IN_3_BYTES = 24;
  constexpr static int BITS_IN_2_BYTES = 16;
  constexpr static int BITS_IN_1_BYTE = 8;

  vector<uint32_t> data;  // Platform-Independent Width
  int capacity;

  BitField(const int &capacity, const vector<uint32_t> &source);
  BitField projectOnto();
  int getMaskFor(const int &size);

 public:
  BitField();
  explicit BitField(const int &capacity);
  void set(int bit);
  void set(const unordered_set<int> &bits);
  void unset(int bit);
  void unset(const unordered_set<int> &bits);
  bool isSet(int bit) const;
  int getCapacity() const;

  bool empty();
  BitField differenceWith(const BitField &other);
  BitField projectOnto(const BitField &other);
  BitField unionWith(const BitField &other);
  bool contains(const BitField &other);

  static BitField copyOf(const BitField *source);
};
