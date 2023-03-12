#include "pch.h"
#include "MyFloat.h"
#include <type_traits>

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

TEST(MyFloatTests, AMyFloatDefinedWithAUserDefinedLiteralRoundTrips)
{
  auto myFloat = 1.0_f;

  // NB: extra parens needed to avoid pre-processor swallowing comma in template argument list
  ASSERT_TRUE((std::is_same_v<MyFloat, decltype(myFloat)>));
  ASSERT_EQ(1.0, myFloat);
}
