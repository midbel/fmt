#ifndef __FMT_FMT_H__
#define __FMT_FMT_H__

#include <iostream>
#include <sstream>
#include "error.hpp"
#include "state.hpp"

namespace fmt {

  template<typename... Args>
  void formatf(std::ostream& os, const std::string& format, const Args&... args) {
    if (!format.size()) {
      return ;
    }
    state sx;
    sx.format(os, format, args...);
  }

  template<typename... Args>
  std::string sformat(const std::string& format, const Args&... args) {
    std::ostringstream os;
    formatf(os, format, args...);
    return os.str();
  }

  template<typename... Args>
  void format(const std::string& format, const Args&... args) {
    formatf(std::cout, format, args...);
  }
}

#endif
