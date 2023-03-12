#include "MyFloat.h"
#include "gtest\gtest.h"
#include <iostream>
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

struct AdditionTestCase
{
  double first;
  double second;
  double result;
};

auto operator<<(std::ostream& os, AdditionTestCase const& value) -> std::ostream&
{
  return os << value.first << " + " << value.second << " = " << value.result;
}

class MyFloatAdditionTests : public ::testing::TestWithParam<AdditionTestCase>
{
};

TEST_P(MyFloatAdditionTests, CheckThatAdditionGivesExpectedResult)
{
  auto const& param = GetParam();
  auto x = MyFloat{ param.first };
  auto y = MyFloat{ param.second };
  auto expectedResult = MyFloat{ param.result };
  ASSERT_EQ(expectedResult, x + y);
}

static const auto AdditionTestCases = ::testing::Values(
  AdditionTestCase{ 0.0, 0.0, 0.0 },
  AdditionTestCase{ 0.0, 1.0, 1.0 },
  AdditionTestCase{ 1.0, 0.0, 1.0 },
  AdditionTestCase{ 1.0, 1.0, 2.0 },
  AdditionTestCase{ 0.5, 0.5, 1.0 }
);

INSTANTIATE_TEST_SUITE_P(AdditionTestCases, MyFloatAdditionTests, AdditionTestCases);

