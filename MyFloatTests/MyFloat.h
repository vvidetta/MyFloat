#ifndef MYFLOAT_H
#define MYFLOAT_H

template <
  typename TOut,
  typename TIn
>
TOut bit_cast(TIn x)
{
  static_assert(sizeof(TOut) == sizeof(TIn));

  TOut y;
  std::memcpy(&y, &x, sizeof(TOut));
  return y;
}

struct MyFloat {

  explicit MyFloat(double x = 0.0)
    : repr{ bit_cast<uint64_t>(x) }
  {
  }

  operator double() const
  {
    return bit_cast<double>(repr);
  }

private:
  uint64_t repr;
};

MyFloat operator ""_f(long double x)
{
  return MyFloat{ static_cast<double>(x) };
}

#endif // MYFLOAT_H
