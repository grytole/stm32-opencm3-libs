#ifndef __MATH_FSQRT_H__
#define __MATH_FSQRT_H__

#include <stdint.h>

float fsqrt(float x);

float fsqrt(float x)
{
  union {
    int32_t i;
    float x;
  } u;

  u.x = x;
  u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
  u.x = u.x + (x / u.x);
  u.x = (0.25 * u.x) + (x / u.x);

  return u.x;
}

#endif /* __MATH_FSQRT_H__ */
