#include "BitField.h"

BitField::BitField(): BitField(0) {}

BitField::BitField(const int &capacity):
    data((capacity / UNIT_SIZE) + (((capacity % UNIT_SIZE) > 0) ? 1 : 0)),
    capacity(capacity) {
  for (int i = 0; i < data.size(); i++) {
    data[i] = 0;
  }
}

void BitField::set(int bit) {
  if (bit >= capacity) {
    return;
  }

  int byteOffset = bit / UNIT_SIZE;
  int bitOffset = bit % UNIT_SIZE;

  data[byteOffset] |= (0x1) << bitOffset;
}

void BitField::unset(int bit) {
  if (bit >= capacity) {
    return;
  }

  int byteOffset = bit / UNIT_SIZE;
  int bitOffset = bit % UNIT_SIZE;

  int setMask = (0x1) << bitOffset;
  data[byteOffset] &= (~setMask);
}

bool BitField::isSet(int bit) const {
  if (bit >= capacity) {
    return false;
  }

  int byteOffset = bit / UNIT_SIZE;
  int bitOffset = bit % UNIT_SIZE;
  int mask = (0x1) << bitOffset;

  return (data[byteOffset] & mask) == mask;
}

bool BitField::empty() {
  for (int x : data) {
    if (x != 0) {
      return false;
    }
  }
  return true;
}

BitField BitField::unionWith(const BitField &other) {
  int mySize = data.size();
  int otherSize = other.data.size();
  int minSize = (mySize < otherSize) ? mySize : otherSize;
  const BitField* biggerField = (mySize > otherSize) ? this : &other;
  int maxCapacity = (capacity > other.capacity) ? capacity : other.capacity;

  BitField result(maxCapacity);
  for (int i = 0; i < minSize; i++) {
    result.data[i] = data[i] | other.data.at(i);
  }

  for (int i = minSize * UNIT_SIZE; i < maxCapacity; i++) {
    if (biggerField->isSet(i)) {
      result.set(i);
    }
  }
  return result;
}

bool BitField::contains(const BitField &other) {
  int mySize = data.size();
  int otherSize = other.data.size();
  if (otherSize > mySize) {
    return false;
  }

  for (int i = 0; i < mySize; i++) {
    if ((data[i] | other.data.at(i)) != data[i]) {
      return false;
    }
  }
  return true;
}

BitField BitField::differenceWith(const BitField &other) {
  int mySize = data.size();
  int otherSize = other.data.size();
  int minSize = (mySize < otherSize) ? mySize : otherSize;
  int maxCapacity = (capacity > other.capacity) ? capacity : other.capacity;

  BitField result(maxCapacity);
  for (int i = 0; i < minSize; i++) {
    result.data[i] = data[i] ^ other.data.at(i);
  }

  for (int i = minSize * UNIT_SIZE; i < maxCapacity; i++) {
    result.set(i);
  }

  return result;
}

BitField BitField::projectOnto(const BitField &other) {
  int maxCapacity = (capacity > other.capacity) ? capacity : other.capacity;
  int minCapacity = (capacity < other.capacity) ? capacity : other.capacity;

  BitField result(maxCapacity);
  for (int i = 0; i < minCapacity; i++) {
    if (other.isSet(i) && !isSet(i)) {
      result.set(i);
    }
  }

  for (int i = minCapacity; i < maxCapacity && i < other.capacity; i++) {
    if (other.isSet(i)) {
      result.set(i);
    }
  }

  return result;
}
