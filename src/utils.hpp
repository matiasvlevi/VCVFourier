#pragma once

#include "plugin.hpp"
#include <vector>

#define TWO_PI 2.f * M_PI
typedef float (*trigFunc)(float);

namespace Utils
{
  std::vector<float> split(std::string const &original, char separator);
  bool tryParse(std::string expression, float &v);
  std::string toText(std::vector<float> data);
  namespace Waves
  {
    float fourier(float phase, std::vector<float> speeds, std::vector<float> lengths, trigFunc trig);
  }
}