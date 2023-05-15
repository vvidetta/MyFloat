#include "MyFloat.h"
#include "gtest/gtest.h"
#include <cfenv>
#include <iostream>
#include <numeric>
#include <ranges>
#include <random>

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


  std::cout << fegetround() << "\n"
    "D = " << FE_DOWNWARD << "\n"
    "N = " << FE_TONEAREST << "\n"
    "Z = " << FE_TOWARDZERO << "\n"
    "U = " << FE_UPWARD << "\n";

#define PRINT_CASE(x) \
  case x: \
  std::cout << #x << '\n'; \
  break

  switch (std::numeric_limits<double>::round_style)
  {
    PRINT_CASE(std::float_round_style::round_indeterminate);
    PRINT_CASE(std::float_round_style::round_toward_infinity);
    PRINT_CASE(std::float_round_style::round_toward_neg_infinity);
    PRINT_CASE(std::float_round_style::round_toward_zero);
    PRINT_CASE(std::float_round_style::round_to_nearest);
  }

  ASSERT_EQ(expectedResult, x + y);
}

static constexpr auto number_of_doubles = 100i64;
static constexpr auto double_seed = 0Ui64;
static auto mt19937 = std::mt19937_64{ double_seed };

[[nodiscard]] static constexpr auto getDoubles() noexcept
{
  auto doubles = std::vector<double>(number_of_doubles);
  std::ranges::generate(doubles, []() -> double { return std::bit_cast<double>(mt19937()); });
  return doubles;
}

[[nodiscard]] static auto getAdditionTestCases() noexcept
{
  auto testCases = std::vector<AdditionTestCase>();
  auto doubles = getDoubles();
  for (auto x : doubles)
    for (auto y : doubles)
      testCases.push_back(AdditionTestCase{ x, y, x + y });
  return testCases;
}

static const auto AdditionTestCases = ::testing::ValuesIn(getAdditionTestCases());

INSTANTIATE_TEST_SUITE_P(AdditionTestCases, MyFloatAdditionTests, AdditionTestCases);

