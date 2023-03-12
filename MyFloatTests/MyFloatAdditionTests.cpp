#include "MyFloat.h"
#include "gtest/gtest.h"
#include <iostream>
#include <ranges>

struct AdditionTestCase
{
  double first;
  double second;
  double result;

  friend auto operator<=>(
    AdditionTestCase const& lhs,
    AdditionTestCase const& rhs
  ) = default;
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

static auto ensureAdditionIsCommutative(std::vector<AdditionTestCase>& testCases) noexcept -> void
{
  for (auto it = std::begin(testCases); it != std::end(testCases); ++it)
  {
    if (it->first == it->second) {
      continue;
    }

    auto reversedCase = AdditionTestCase{ it->second, it->first, it->result };
    if (std::ranges::find(testCases, reversedCase) != std::ranges::end(testCases)) {
      continue;
    }

    it = testCases.insert(it + 1, reversedCase);
  }
}

[[nodiscard]] static auto getAdditionTestCases() noexcept
{
  auto testCases = std::vector{
    AdditionTestCase{ 0.0, 0.0, 0.0 },
    AdditionTestCase{ 0.0, 1.0, 1.0 },
    AdditionTestCase{ 1.0, 1.0, 2.0 },
    AdditionTestCase{ 0.5, 0.5, 1.0 }
  };
  ensureAdditionIsCommutative(testCases);
  return testCases;
}

static const auto AdditionTestCases = ::testing::ValuesIn(getAdditionTestCases());

INSTANTIATE_TEST_SUITE_P(AdditionTestCases, MyFloatAdditionTests, AdditionTestCases);

