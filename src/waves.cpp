#include "utils.hpp"

namespace Utils::Waves
{
  /*
   *  TODO: Implement chebyshev polynomials
   */
  float fourier(
      float phase,
      float &sum,
      std::vector<float> freq,
      std::vector<float> amps,
      trigFunc trig)
  {
    sum = 0.f;
    if (freq.size() == amps.size())
    {
      for (short int n = 0; n < (short int)freq.size(); n++)
        sum += trig(TWO_PI * phase * freq[n]) / amps[n];
    }
    else if (freq.size() > amps.size())
    {
      for (short int n = 0; n < (short int)freq.size(); n++)
        sum += trig(TWO_PI * phase * freq[n]) / (n + 1);
    }
    else if (freq.size() < amps.size())
    {
      for (short int n = 0; n < (short int)amps.size(); n++)
        sum += trig(TWO_PI * phase * (n + 1)) / amps[n];
    }
    return sum;
  };
};