#ifndef __FMT_FMT_H__
#define __FMT_FMT_H__

#include <any>
#include <bitset>
#include <exception>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#define BIN 2
#define OCT 8
#define DEC 10
#define HEX 16

#define STRING "string"
#define INT "int"
#define DOUBLE "double"
#define BOOL "bool"
#define CHAR "char"

#define PREC 6

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

  // syntax: %[[index]][#][-][ |0][width.prec][+][verb]
  class state {
  public:
    template<typename... Args>
    void format(std::ostream& os, std::string format, Args&... args) {
      std::vector<std::any> values = {args...};
      int argnum = 0;

      auto it = format.begin();
      auto end = format.end();
      while (it < end) {
        format_text(os, it, end);
        if (it == end) {
          break;
        }
        reset();

        it = std::next(it);
        parse_specifier(argnum, it, format.end());
        if (arg < 0 || arg >= values.size()) {
          throw bad_index();
        }
        auto val = values.at(arg);
        switch (*it) {
          case 'c':
          format_char(os, val);
          break;
          case 's':
          format_string(os, val, false);
          break;
          case 'q':
          format_string(os, val, true);
          break;
          case 'd':
          format_int(os, val);
          break;
          case 'b':
          format_int(os, val, BIN);
          break;
          case 'o':
          format_int(os, val, OCT);
          break;
          case 'f':
          format_double(os, val);
          break;
          case 'p':
          format_double(os, val, true);
          break;
          case 'e':
          format_double(os, val, false, true);
          break;
          case 't':
          format_bool(os, val);
          break;
          case 'x':
          format_hex(os, val);
          break;
          case 'v':
          format_value(os, val);
          break;
          case 'T':
          format_type(os, val);
          break;
          default:
          throw bad_verb();
          break;
        }
        argnum++;
        it = next(it);
      }
      os << std::resetiosflags(std::ios_base::basefield);
      os << std::flush;
    }

    void reset() {
      arg = 0;
      wid = 0;
      prec = PREC;

      sign = false;
      space = false;
      zeros = false;
      prefix = false;
      sharp = false;
    }

  private:
    int arg;
    int wid;
    int prec;

    bool sign;
    bool space;
    bool zeros;
    bool prefix;
    bool sharp;

    void format_int(std::ostream& os, std::any val, int base = DEC) {
      if (val.type() != typeid(int))   {
        throw bad_argument();
      }
      int i = std::any_cast<int>(val);
      if (sign && i > 0) {
        os << fmt::plus;
      }
      os << (sharp ? std::showbase : std::noshowbase);
      switch (base) {
        case BIN:
        os << std::bitset<32>(i).to_string();
        return ;
        case OCT:
        os << std::oct;
        break;
        case HEX:
        os << std::hex;
        break;
        default:
        os << std::dec;
        break;
      }
      os << i;
    }

    void format_string(std::ostream& os, std::any val, bool quote = false) {
      std::string str;
      if (val.type() == typeid(std::string)) {
        str = std::any_cast<std::string>(val);
      } else if (val.type() == typeid(const char*)) {
        str = std::string(std::any_cast<const char*>(val));
      } else if (val.type() == typeid(char*)) {
        str = std::string(std::any_cast<char*>(val));
      } else {
        throw bad_argument();
      }
      if (quote) {
        os << std::quoted(str);
      } else {
        os << str;
      }
    }

    void format_double(std::ostream& os, std::any val, bool percent = false, bool exponent = false) {
      double i;
      if (val.type() == typeid(double)) {
        i = std::any_cast<double>(val);
      } else if (val.type() == typeid(int)) {
        i = std::any_cast<int>(val);
      } else {
        throw bad_argument();
      }
      if (sign && i > 0) {
        os << fmt::plus;
      }
      if (exponent) {
        os << std::scientific << std::setprecision(prec) << i;
        return ;
      }
      if (percent) {
        os << std::setprecision(prec) << i*100 << "%";
        return ;
      }
      os << std::setprecision(prec) << i;
    }

    void format_bool(std::ostream& os, std::any val) {
      if (val.type() != typeid(bool)) {
        throw bad_argument();
      }
      os << (sharp ? std::noboolalpha : std::boolalpha);
      os << std::any_cast<bool>(val);
    }

    void format_char(std::ostream& os, std::any val) {
      if (val.type() != typeid(char)) {
        throw bad_argument();
      }
      os << std::any_cast<char>(val);
    }

    void format_hex(std::ostream& os, std::any val) {
      if (val.type() == typeid(std::string)) {

      } else if (val.type() == typeid(char*)) {

      } else if (val.type() == typeid(int)) {
        format_int(os, val, HEX);
      } else if (val.type() == typeid(double)) {

      } else if (val.type() == typeid(char)) {
      } else {
        throw bad_argument();
      }
    }

    void format_type(std::ostream& os, std::any val) {
      std::string type;
      if (val.type() == typeid(std::string)) {
        type = STRING;
      } else if (val.type() == typeid(char*)) {
        type = STRING;
      } else if (val.type() == typeid(int)) {
        type = INT;
      } else if (val.type() == typeid(double)) {
        type = DOUBLE;
      } else if (val.type() == typeid(bool)) {
        type = BOOL;
      } else if (val.type() == typeid(char)) {
        type = CHAR;
      } else {
        throw bad_argument();
      }
      os << type;
    }

    void format_value(std::ostream& os, std::any val) {
      if (val.type() == typeid(std::string)) {
        format_string(os, val, false);
      } else if (val.type() == typeid(char*)) {
        format_string(os, val, false);
      } else if (val.type() == typeid(int)) {
        format_int(os, val);
      } else if (val.type() == typeid(double)) {
        format_double(os, val);
      } else if (val.type() == typeid(bool)) {
        format_bool(os, val);
      } else if (val.type() == typeid(char)) {
        format_char(os, val);
      } else {
        throw bad_argument();
      }
    }

    void format_text(std::ostream& os, std::string::iterator& it, const std::string::iterator& end) {
      auto prev = it;
      while (it < end) {
        if (*it == fmt::percent) {
          if (*(it+1) != fmt::percent) {
            break;
          }
          os << std::string(prev, it);
          it = next(it);
          prev = it;
        }
        it = next(it);
      }
      os << std::string(prev, it);
    }

    char parse_specifier(int ix, std::string::iterator& it, const std::string::iterator& end) {
      parse_index(ix, it, end);

      if (*it == fmt::pound) {
        sharp = true;
        it = std::next(it);
      }
      if (*it == fmt::minus) {
        prefix = true;
        it = std::next(it);
      }
      if (*it == fmt::space) {
        space = true;
        skip_chars(fmt::space, it, end);
      }
      if (!space && *it == fmt::zero) {
        zeros = true;
        skip_chars(fmt::zero, it, end);
      }
      if (*it == fmt::star) {
        it = std::next(it);
      } else {
        parse_width(it, end);
      }
      if (*it == fmt::star) {
        it = std::next(it);
      } else {
        parse_precision(it);
      }

      if (*it == fmt::plus) {
        sign = true;
        it = std::next(it);
      }
      return *it;
    }

    void parse_index(int ix, std::string::iterator& start, const std::string::iterator& end) {
      if (*start != lsquare) {
        arg = ix;
        return;
      }
      auto it = next(start);
      while (it < end && *it != rsquare) {
        if (!is_number(*it)) {
          throw bad_argument();
        }
        it = std::next(it);
      }
      it = std::next(it);
      arg = std::stoi(std::string(start+1, it-1))-1;
      start = it;
    }

    void parse_width(std::string::iterator& it, const std::string::iterator& end) {
      if (!is_number(*it)) {
        return;
      }
      skip_chars(zero, it, end);
      auto start = it;
      while (is_number(*it)) {
        it = std::next(it);
      }
      wid = std::stoi(std::string(start, it));
    }

    void parse_precision(std::string::iterator& it) {
      if (*it != dot) {
        return;
      }
      it = next(it);
      auto start = it;
      while (is_number(*it)) {
        it = std::next(it);
      }
      prec = std::stoi(std::string(start, it));
      if (prec == 0) {
        prec = PREC;
      }
    }

    bool is_number(char c) {
      return c >= '0' && c <= '9';
    }

    void skip_chars(char c, std::string::iterator& start, const std::string::iterator& end) {
      while (start < end && *start == c) {
        start = std::next(start);
      }
    }
  } sx;

  template<typename... Args>
  void formatf(std::ostream& os, const std::string& format, const Args&... args) {
    if (!format.size()) {
      return ;
    }
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
