#include <any>
#include <vector>

#include "fmt.hpp"

namespace fmt
{

  const char percent = '%';
  const char lsquare = '[';
  const char rsquare = ']';
  const char dot = '.';
  const char pound = '#';
  const char space = ' ';
  const char star = '*';
  const char plus = '+';
  const char minus = '-';
  const char zero = '0';

  template<typename... Args>
  void format_string(ostream& os, const std::string& format, Args... args);

  template<typename... Args>
  std::string sprintf(const std::string& format, Args... args)
  {
    ostringstream os;
    format_string(os, format, args...);
    return os.str();
  }

  template<typename... Args>
  void printf(const std::string& format, Args... args)
  {
    fprintf(cout, format, args...)
  }

  template<typename... Args>
  void fprintf(ostream& os, const std::string& format, Args... args)
  {
    format_string(os, format, args...);
  }

  template<typename... Args>
  void format_string(ostream& os, const std::string& format, Args... args)
  {
    if (!format.size()) {
      return ;
    }
    vector<any> values = {args...};
    int arg = 0;

    auto it = format.begin();
    auto end = format.end();
    auto prev = it;
    while (it < end)
    {

    }
  }
}
