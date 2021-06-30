#ifndef __MATH_SQRTF_H__
#define __MATH_SQRTF_H__

#include <stdint.h>

float sqrtf(float x);

float sqrtf(float x)
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

#endif /* __MATH_SQRTF_H__ */
