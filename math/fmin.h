#ifndef __MATH_FMIN_H__
#define __MATH_FMIN_H__

float fmin(float x, float y);

float fmin(float x, float y)
{
  return ((x < y) ? x : y);
}

#endif /* __MATH_FMIN_H__ */
