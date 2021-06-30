#ifndef __MATH_FABSF_H__
#define __MATH_FABSF_H__

#include <stdint.h>

float fabsf(float x);

float fabsf(float x)
{
  union {
    float x;
    uint32_t i;
  } u;

  u.x = x;
  u.i &= 0x7fffffff;

  return u.x;
}

#endif /* __MATH_FABSF_H__ */
