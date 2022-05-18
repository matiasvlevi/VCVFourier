#include "utils.hpp"

namespace Utils
{
  std::vector<float> split(std::string const &str, char delim)
  {
    float val = 0.f;
    std::vector<float> ans;
    std::string::const_iterator start = str.begin();
    std::string::const_iterator end = str.end();
    std::string::const_iterator next = std::find(start, end, delim);

    if (str.length() == 0)
      return {};

    while (next != end)
    {
      if (tryParse(std::string(start, next), val))
      {
        ans.push_back(val);
      }
      start = next + 1;
      next = std::find(start, end, delim);
    }
    if (tryParse(std::string(start, next), val))
    {
      ans.push_back(val);
    }
    return ans;
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