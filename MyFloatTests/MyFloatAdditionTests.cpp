#include "MyFloat.h"
#include "gtest/gtest.h"
#include <iostream>

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

