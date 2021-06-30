#ifndef __MATH_FMINF_H__
#define __MATH_FMINF_H__

float fminf(float x, float y);

float fminf(float x, float y)
{
  return ((x < y) ? x : y);
}

#endif /* __MATH_FMINF_H__ */
