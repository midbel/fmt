#include <iostream>
#include <exception>

namespace fmt
{
  class fmt_error: public std::exception {
  private:
    string msg;
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

  class printer {
  private:
    // vector<> nodes;
  public:
    formatter(string format);
    template<typename... Args>
    string sprintf(Args... args);
    template<typename... Args>
    void printf(Args... args);
    template<typename... Args>
    void fprintf(std::ostream& os, Args... args);
  }

  template<typename... Args>
  std::string sprintf(const std::string& format, Args... args);

  template<typename... Args>
  void printf(const std::string& format, Args... args);

  template<typename... Args>
  void fprintf(std::ostream& os, const std::string& format, Args...args);
}
