#include "utils.hpp"

namespace Utils
{
  std::vector<float> split(std::string const &original, char separator)
  {
    float val = 0.f;
    std::vector<float> results;
    std::string::const_iterator start = original.begin();
    std::string::const_iterator end = original.end();
    std::string::const_iterator next = std::find(start, end, separator);

    while (next != end)
    {
      if (tryParse(std::string(start, next), val))
      {
        results.push_back(val);
      }
      start = next + 1;
      next = std::find(start, end, separator);
    }
    if (tryParse(std::string(start, next), val))
    {
      results.push_back(val);
    }
    return results;
  }

  bool tryParse(std::string expression, float &v)
  {
    char *end;
    float value = strtof(expression.c_str(), &end);

    if (*end == 0)
    {
      v = value;
      return true;
    }

    return false;
  };

  std::string toText(std::vector<float> data)
  {
    std::string result = "";
    for (short i = 0; i < (short)data.size(); i++)
    {
      result += std::to_string(data[i]) + ",";
    }
    return result;
  }
}