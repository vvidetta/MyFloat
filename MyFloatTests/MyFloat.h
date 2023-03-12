#ifndef MYFLOAT_H
#define MYFLOAT_H

struct MyFloat {
  operator double() const {
    return 0.0;
  }
};

#endif // MYFLOAT_H
