#ifndef __FMT_H__
#define __FMT_H__

#include <any>
#include <iostream>
#include <exception>
#include <vector>

#define BIN 2
#define OCT 8
#define DEC 10
#define HEX 16

namespace fmt
{
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

  class state {
  public:
    virtual void print(std::ostream& os, std::any value) = 0;
  };

  class literal {
  public:
    virtual void print(std::ostream& os, std::any value);
  };

  class placeholder {
  public:
    virtual void print(std::ostream& os, std::any value);
  };

  class printer {
  public:
    printer(std::string format);

    template<typename... Args>
    std::string sprintf(Args... args);
    template<typename... Args>
    void printf(Args... args);
    template<typename... Args>
    void fprintf(std::ostream& os, Args... args);

  private:
    std::vector<state> states;

    void parse(std::string format);
  };

  template<typename... Args>
  std::string sprintf(const std::string& format, Args... args);

  template<typename... Args>
  void printf(const std::string& format, Args... args);

  template<typename... Args>
  void fprintf(std::ostream& os, const std::string& format, Args...args);
}

#endif
