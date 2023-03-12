#include "pch.h"
#include "MyFloat.h"

TEST(MyFloatTests, ADefaultConstructedMyFloatIsZero)
{
  auto myFloat = MyFloat{};
  ASSERT_EQ(0.0, myFloat);
}

TEST(MyFloatTests, AMyFloatConstructedWithADoubleRoundTrips)
{
  auto myFloat = MyFloat{ 1.0 };
  ASSERT_EQ(1.0, myFloat);
}
