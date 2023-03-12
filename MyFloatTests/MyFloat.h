#ifndef MYFLOAT_H
#define MYFLOAT_H

#include <bit>
#include <climits>
#include <numeric>

struct MyFloat {

  constexpr explicit MyFloat(double x = 0.0)
    : repr{ std::bit_cast<uint64_t>(x) }
  {
  }

  [[nodiscard]] double to_double() const
  {
    return std::bit_cast<double>(repr);
  }

private:
  uint64_t repr;

  [[nodiscard]] static constexpr auto field_mask(size_t pos, size_t width) -> uint64_t
  {
    return ((1Ui64 << width) - 1Ui64) << pos;
  }

  [[nodiscard]] static constexpr auto extract_field(uint64_t x, size_t pos, size_t width) -> uint64_t
  {
    auto mask = field_mask(pos, width);
    return (x & mask) >> pos;
  }

  static constexpr size_t man_pos = 0;
  static constexpr size_t man_width = 52;

  [[nodiscard]] constexpr auto mantissa() -> uint64_t
  {
    return extract_field(repr, man_pos, man_width);
  }

  static constexpr int exp_pos = man_pos + man_width;
  static constexpr int exp_width = 11;

  [[nodiscard]] constexpr auto exponent() -> uint64_t
  {
    return extract_field(repr, exp_pos, exp_width);
  }

  static constexpr int sign_pos = exp_pos + exp_width;
  static constexpr int sign_width = 1;

  [[nodiscard]] constexpr auto signum() -> uint64_t
  {
    return extract_field(repr, sign_pos, sign_width);
  }

  static_assert(sign_pos + sign_width == sizeof(uint64_t) * CHAR_BIT);

  explicit MyFloat(uint64_t signum, uint64_t exp, uint64_t man)
    : repr{ (signum << sign_pos) | (exp << exp_pos) | (man << man_pos) }
  {
  }

  friend bool operator==(MyFloat lhs, MyFloat rhs)
  {
    return lhs.repr == rhs.repr;
  }

  friend bool operator!= (MyFloat lhs, MyFloat rhs)
  {
    return !(lhs == rhs);
  }

  friend auto operator+(MyFloat x) -> MyFloat
  {
    return x;
  }

  [[nodiscard]] static constexpr auto top_bit(uint64_t x) noexcept -> size_t
  {
    if (x == 0)
    {
      return -1;
    }

    auto top = size_t{ 0 };
    while (x != 1) {
      x >>= 1;
      ++top;
    }
    return top;
  }

  static const auto man_1 = (1Ui64 << man_width) - 1;

  static constexpr auto unset_bit(uint64_t& x, size_t pos) noexcept -> void
  {
    x &= static_cast<uint64_t>(x) ^ (1Ui64 << pos);
  }

  friend auto operator+(MyFloat x, MyFloat y) -> MyFloat
  {
    if (x.repr == 0Ui64) {
      return y;
    } else if (y.repr == 0Ui64) {
      return x;
    }

    auto mx = x.mantissa();
    mx |= 1Ui64 << man_width;
    mx <<= (exp_width - 2Ui64);

    auto my = y.mantissa();
    my |= 1Ui64 << man_width;
    my <<= (exp_width - 2Ui64);

    auto ex = x.exponent();
    ex -= (exp_width - 2Ui64);

    auto ey = y.exponent();
    ey -= (exp_width - 2Ui64);

    if (ex > ey) {
      my >>= ex - ey;
      ey = ex;
    } else if (ey > ex) {
      mx >>= ey - ex;
      ex = ey;
    }

    if (x.signum() == 1Ui64) {
      mx = (~mx) ^ 1Ui64;
    }
    if (y.signum() == 1Ui64) {
      my = (~my) ^ 1Ui64;
    }

    auto mr = mx + my;

    auto sr = extract_field(mr, sign_pos, sign_width);

    if (sr == 1Ui64)
      mr = (~mr) ^ 1Ui64;
    auto shift = top_bit(mr) - man_width;
    auto er = ex + shift;

    mr >>= shift;
    unset_bit(mr, man_width);

    return MyFloat{ sr, er, mr };
  }
};

namespace MyFloat_literals {
  static MyFloat operator ""_f(long double x)
  {
    return MyFloat{ static_cast<double>(x) };
  }
}

#endif // MYFLOAT_H
