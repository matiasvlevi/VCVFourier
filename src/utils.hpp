#include "plugin.hpp"
#include <vector>

namespace Utils
{
  std::vector<float> split(std::string const &original, char separator);
  bool tryParse(std::string expression, float &v);
  std::string toText(std::vector<float> data);
}