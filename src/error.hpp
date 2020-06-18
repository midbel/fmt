#ifndef __FMT_ERROR_H__
#define __FMT_ERROR_H__

#include <exception>

namespace fmt {
  class fmt_error: public std::exception {
  private:
    std::string msg;
  public:
    fmt_error(std::string m): msg(m) {}
    virtual ~fmt_error() {}

    virtual const char* what() const throw() {
      return msg.c_str();
    }
  };

  class bad_verb: public fmt_error {
  public:
    bad_verb(): fmt_error("unsupported verb") {}
  };

  class bad_argument: public fmt_error {
  public:
    bad_argument(): fmt_error("bad argument") {}
  };

  class bad_index: public fmt_error {
  public:
    bad_index(): fmt_error("index out of range") {}
  };
}

#endif
