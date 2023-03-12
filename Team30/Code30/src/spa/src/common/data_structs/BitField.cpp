#include "BitField.h"

BitField::BitField(int capacity):
    data((capacity / 8) + (((capacity % 8) > 0) ? 1 : 0)) {
  for (int i = 0; i < data.size(); i++) {
    data[i] = 0;
  }
}

void BitField::set(int bit) {
  int byteOffset = bit / 32;
  int bitOffset = bit % 32;

  data[byteOffset] |= (0x1) << bitOffset;
}

void BitField::unset(int bit) {
  int byteOffset = bit / 32;
  int bitOffset = bit % 32;

  int setMask = (0x1) << bitOffset;
  data[byteOffset] &= (~setMask);
}

bool BitField::isSet(int bit) {
  int byteOffset = bit / 32;
  int bitOffset = bit % 32;
  int mask = (0x1) << bitOffset;

  return (data[byteOffset] & mask) == mask;
}
