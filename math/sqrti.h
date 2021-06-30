#ifndef __MATH_SQRTI_H__
#define __MATH_SQRTI_H__

#include <stdint.h>

uint32_t sqrti(uint32_t x);

uint32_t sqrti(uint32_t x)
{
  uint32_t arg = x;
  uint32_t one = (1 << 30);
  uint32_t res = 0;

  while (one > arg)
  {
    one >>= 2;
  }

  while (0 != one)
  {
    if (arg >= (res + one))
    {
      arg -= res + one;
      res += 2 * one;
    }
    res >>= 1;
    one >>= 2;
  }

  return res;
}

#endif /* __MATH_SQRTI_H__ */
