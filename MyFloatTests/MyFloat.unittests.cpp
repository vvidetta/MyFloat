#include "MyFloat.h"
#include "gtest\gtest.h"
#include <type_traits>
#include <thread>

TEST(MyFloatTests, ADefaultConstructedMyFloatIsZero)
{
  auto myFloat = MyFloat{};
  ASSERT_EQ(0.0, myFloat.to_double());
}

TEST(MyFloatTests, AMyFloatConstructedWithADoubleRoundTrips)
{
  auto myFloat = MyFloat{ 1.0 };
  ASSERT_EQ(1.0, myFloat.to_double());
}

TEST(MyFloatTests, AMyFloatDefinedWithAUserDefinedLiteralRoundTrips)
{
  auto myFloat = 1.0_f;

  // NB: extra parens needed to avoid pre-processor swallowing comma in template argument list
  ASSERT_TRUE((std::is_same_v<MyFloat, decltype(myFloat)>));
  ASSERT_EQ(1.0, myFloat.to_double());
}

TEST(MyFloatTests, MyFloatEquality)
{
  auto a = 42.0_f;
  ASSERT_TRUE(a == a);

  auto b = 42.0_f;
  ASSERT_TRUE(a == b);

  auto c = MyFloat{ -42.0 };
  ASSERT_FALSE(a == c);
}

TEST(MyFloatTests, MyFloatInequality)
{
  auto a = 42.0_f;
  ASSERT_FALSE(a != a);

  auto b = 42.0_f;
  ASSERT_FALSE(a != b);

  auto c = MyFloat{ -42.0 };
  ASSERT_TRUE(a != c);
}

TEST(MyFloatTests, PositationLeavesMyFloatUnchanged)
{
  auto a = 42.0_f;
  ASSERT_EQ(a, +a);
}

TEST(MyFloatTests, MyFloatAddition)
{
  //std::this_thread::sleep_for(std::chrono::seconds(10));
  ASSERT_EQ(0.0_f + 0.0_f, 0.0_f);
  ASSERT_EQ(0.0_f + 1.0_f, 1.0_f);
  ASSERT_EQ(1.0_f + 0.0_f, 1.0_f);
  ASSERT_EQ(1.0_f + 1.0_f, 2.0_f);
  // ASSERT_EQ(42.0_f + 75.0_f, 117.0_f);
}

