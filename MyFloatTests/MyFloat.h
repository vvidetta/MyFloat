#ifndef MYFLOAT_H
#define MYFLOAT_H

#include <bit>
#include <climits>

struct MyFloat {

  explicit MyFloat(double x = 0.0)
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

    auto my = y.mantissa();
    my |= 1Ui64 << man_width;

    auto mr = mx + my;

    auto shift = top_bit(mr) - man_width;
    mr >>= shift;
    unset_bit(mr, man_width);

    auto er = x.exponent() + shift;

    return MyFloat{ 0Ui64, er, mr };

    //auto mx = x.mantissa();
    //auto ex = x.exponent();
    //auto my = y.mantissa();
    //auto ey = y.exponent();

    //mx |= 1Ui64 << man_width;
    //my |= 1Ui64 << man_width;

    //if (ex == ey)
    //{
    //  auto mr = (mx + my) >> 1;
    //  return MyFloat{ x.signum(), ex, mr };
    //} else if (ex > ey) {
    //  if (ex - ey < exp_width)
    //  {
    //    auto wx = man_width + 1 + (ex - ey);
    //    mx <<= (ex - ey);
    //    mx += my;
    //    auto de = (top_bit(mx) - (man_width + 1));
    //    mx >>= de;
    //    ex = ey + de;
    //    return MyFloat{ x.signum(), ex, mx };
    //  }
    //  else {
    //    return MyFloat{ }; // TODO
    //  }
    //}

    //return MyFloat{}; // TODO
  }
};

namespace MyFloat_literals {
  static MyFloat operator ""_f(long double x)
  {
    return MyFloat{ static_cast<double>(x) };
  }
}

#endif // MYFLOAT_H
