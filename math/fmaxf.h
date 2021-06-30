#ifndef __MATH_FMAXF_H__
#define __MATH_FMAXF_H__

float fmaxf(float x, float y);

float fmaxf(float x, float y)
{
  return ((x > y) ? x : y);
}

#endif /* __MATH_FMAXF_H__ */
