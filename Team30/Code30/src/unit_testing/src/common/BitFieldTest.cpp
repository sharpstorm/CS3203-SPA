// #define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "catch.hpp"
#include "common/data_structs/BitField.h"

void populateBitfield(BitField *target, int start, int max) {
  for (int i = start; i < max; i++) {
    target->set(i);
  }
}

void populateBitfield(BitField *target, int count) {
  populateBitfield(target, 0, count);
}

void requireBitfieldTrue(BitField *target, int min, int max) {
  for (int i = min; i < max; i++) {
    REQUIRE(target->isSet(i));
  }
}

void requireBitfieldTrue(BitField *target, int max) {
  requireBitfieldTrue(target, 0, max);
}

void requireBitfieldFalse(BitField *target, int min, int max) {
  for (int i = min; i < max; i++) {
    REQUIRE_FALSE(target->isSet(i));
  }
}

void requireBitfieldFalse(BitField *target, int max) {
  requireBitfieldFalse(target, 0, max);
}

void requireBitfieldLength(BitField *target, int size) {
  REQUIRE(target->getCapacity() == size);
}

TEST_CASE("Bit Field Project Onto") {
  BitField bfA(33);
  BitField bfB(33);

  populateBitfield(&bfA, 5);
  populateBitfield(&bfB, 33);

  auto result = bfA.projectOnto(bfB);
  requireBitfieldFalse(&result, 5);
  requireBitfieldTrue(&result, 5, 33);
  requireBitfieldLength(&result, 33);

  bfA = BitField(33);
  bfB = BitField(32);
  populateBitfield(&bfA, 5);
  populateBitfield(&bfB, 12);

  result = bfA.projectOnto(bfB);
  requireBitfieldFalse(&result, 5);
  requireBitfieldTrue(&result, 5, 12);
  requireBitfieldLength(&result, 32);

  bfA = BitField(32);
  bfB = BitField(33);
  populateBitfield(&bfA, 5);
  populateBitfield(&bfB, 33);

  result = bfA.projectOnto(bfB);
  requireBitfieldFalse(&result, 5);
  requireBitfieldTrue(&result, 5, 10);
  requireBitfieldLength(&result, 33);

  bfA = BitField(32);
  bfB = BitField(32);
  populateBitfield(&bfA, 14);
  populateBitfield(&bfB, 32);

  result = bfA.projectOnto(bfB);
  requireBitfieldFalse(&result, 14);
  requireBitfieldTrue(&result, 14, 32);
  requireBitfieldLength(&result, 32);
}

TEST_CASE("Bit Field Union Onto") {
  BitField bfA(33);
  BitField bfB(33);

  populateBitfield(&bfA, 16);
  populateBitfield(&bfB, 16, 32);

  auto result = bfA.unionWith(bfB);
  requireBitfieldTrue(&result, 32);
  requireBitfieldFalse(&result, 32, 32);
  requireBitfieldLength(&result, 33);

  bfA = BitField(32);
  bfB = BitField(33);
  populateBitfield(&bfA, 5);
  populateBitfield(&bfB, 4, 32);

  result = bfA.unionWith(bfB);
  requireBitfieldTrue(&result, 32);
  requireBitfieldFalse(&result, 32, 32);
  requireBitfieldLength(&result, 33);

  bfA = BitField(33);
  bfB = BitField(32);
  populateBitfield(&bfA, 5);
  populateBitfield(&bfB, 6, 32);

  result = bfA.unionWith(bfB);
  requireBitfieldTrue(&result, 5);
  requireBitfieldFalse(&result, 5, 5);
  requireBitfieldTrue(&result, 6, 32);
  requireBitfieldFalse(&result, 32, 32);
  requireBitfieldLength(&result, 33);

  bfA = BitField(32);
  bfB = BitField(32);
  populateBitfield(&bfA, 5);
  populateBitfield(&bfB, 16);

  result = bfA.projectOnto(bfB);
  requireBitfieldFalse(&result, 5);
  requireBitfieldTrue(&result, 5, 16);
  requireBitfieldLength(&result, 32);
}

#ifdef CATCH_CONFIG_ENABLE_BENCHMARKING

TEST_CASE("Benchmark bitfield performance") {
  BitField bfA(500);
  BitField bfB(500);

  populateBitfield(&bfA, 400);
  populateBitfield(&bfB, 350, 500);

  BENCHMARK("Bench Bitfield Projection") -> void {
      auto result = bfA.projectOnto(bfB);
    };

  BENCHMARK("Bench Bitfield Union") -> void {
      auto result = bfA.unionWith(bfB);
    };
}

#endif
