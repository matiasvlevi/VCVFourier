#include "utils.hpp"

namespace Utils::Waves
{
  /*
   *  TODO: Implement chebyshev polynomials
   */
  float fourier(
      float phase,
      std::vector<float> speeds,
      std::vector<float> lengths,
      trigFunc trig)
  {
    float sum = 0.f;
    if (speeds.size() == lengths.size())
    {
      for (short int n = 0; n < (short int)speeds.size(); n++)
        sum += trig(TWO_PI * phase * speeds[n]) / lengths[n];
    }
    else if (speeds.size() > lengths.size())
    {
      for (short int n = 0; n < (short int)speeds.size(); n++)
        sum += trig(TWO_PI * phase * speeds[n]) / (n + 1);
    }
    else if (speeds.size() < lengths.size())
    {
      for (short int n = 0; n < (short int)lengths.size(); n++)
        sum += trig(TWO_PI * phase * (n + 1)) / lengths[n];
    }
    return sum;
  };
};