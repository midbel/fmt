#include <sstream>
#include "fmt.hpp"

namespace fmt {

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
  void format_string(std::ostream& os, const std::string& format, Args... args) {
    os << format;
  }

  template<typename... Args>
  std::string sprintf(const std::string& format, Args... args) {
    std::ostringstream os;
    fprintf(os, format, args...);
    return os.str();
  }

  template<typename... Args>
  void printf(const std::string& format, Args... args) {
    fprintf(std::cout, format, args...);
  }

  template<typename... Args>
  void fprintf(std::ostream& os, const std::string& format, Args... args) {
    format_string(os, format, args...);
  }
}
