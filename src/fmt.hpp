#include <iostream>

namespace fmt
{
  template<typename... Args>
  std::string sprintf(const std::string& format, Args... args);

  template<typename... Args>
  void printf(const std::string& format, Args... args);

  template<typename... Args>
  void fprintf(std::ostream& os, const std::string& format, Args...args);
}
