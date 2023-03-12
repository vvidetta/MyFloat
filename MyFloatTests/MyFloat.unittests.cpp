#include "pch.h"
#include "MyFloat.h"

TEST(MyFloatTests, ADefaultConstructedMyFloatIsZero) {
  auto myFloat = MyFloat{};
  ASSERT_EQ(0.0, myFloat);
}

