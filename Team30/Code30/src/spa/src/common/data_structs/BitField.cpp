#include "BitField.h"

BitField::BitField() : BitField(0) {}

BitField::BitField(const int &capacity) :
    data((capacity / UNIT_SIZE) + (((capacity % UNIT_SIZE) > 0) ? 1 : 0)),
    capacity(capacity) {
  for (int i = 0; i < data.size(); i++) {
    data[i] = 0;
  }
}

BitField::BitField(const int &capacity, const vector<uint32_t> &source) :
    data((capacity / UNIT_SIZE) + (((capacity % UNIT_SIZE) > 0) ? 1 : 0)),
    capacity(capacity) {
  for (int i = 0; i < data.size(); i++) {
    data[i] = source[i];
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
  int mySize = capacity;
  int otherSize = other.capacity;
  const BitField *biggerField = (mySize > otherSize) ? this : &other;
  const BitField *smallerField = (mySize > otherSize) ? &other : this;

  BitField result = BitField::copyOf(biggerField);
  int numCopy = smallerField->capacity / UNIT_SIZE;
  int bitsCopied = numCopy * UNIT_SIZE;
  for (int i = 0; i < numCopy; i++) {
    result.data[i] |= smallerField->data[i];
  }

  if (smallerField->capacity > bitsCopied) {
    result.data[numCopy] |= smallerField->data[numCopy];
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
  const int minCap = (capacity < other.capacity) ? capacity : other.capacity;

  BitField result = copyOf(&other);
  int numCopy = minCap / UNIT_SIZE;
  int bitsCopied = numCopy * UNIT_SIZE;
  for (int i = 0; i < numCopy; i++) {
    result.data[i] = other.data[i] & ~data[i];
  }

  if (capacity > bitsCopied && other.capacity > bitsCopied) {
    int myMask = getMaskFor(capacity - bitsCopied);
    int otherMask = getMaskFor(other.capacity - bitsCopied);
    result.data[numCopy] = (otherMask & other.data[numCopy])
        & ~(myMask & data[numCopy]);
  }

  return result;
}

BitField BitField::copyOf(const BitField *source) {
  return BitField(source->capacity, source->data);
}

int BitField::getMaskFor(const int &size) {
  if (size >= 32) {
    return FULL_MASK_LOWER_32;
  }
  switch (size / 8) {
    case 3:
      return (MASK_8BITS[size - BITS_IN_3_BYTES] << BITS_IN_3_BYTES)
          | FULL_MASK_LOWER_24;
    case 2:
      return (MASK_8BITS[size - BITS_IN_2_BYTES] << BITS_IN_2_BYTES)
          | FULL_MASK_LOWER_16;
    case 1:
      return (MASK_8BITS[size - BITS_IN_1_BYTE] << BITS_IN_1_BYTE)
          | FULL_MASK_LOWER_8;
    default:return MASK_8BITS[size];
  }
}

int BitField::getCapacity() const {
  return capacity;
}
