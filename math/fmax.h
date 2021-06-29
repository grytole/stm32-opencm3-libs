#ifndef __MATH_FMAX_H__
#define __MATH_FMAX_H__

float fmax(float x, float y);

float fmax(float x, float y)
{
  return ((x > y) ? x : y);
}

#endif /* __MATH_FMAX_H__ */
