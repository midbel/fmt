#ifndef __FMT_STATE_H__
#define __FMT_STATE_H__

#include <vector>
#include <any>
#include <iostream>
#include <iomanip>
#include <bitset>
#include "error.hpp"

#define BIN 2
#define OCT 8
#define DEC 10
#define HEX 16

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

  // syntax: %[[index]][#][-][ |0][width.prec][+][verb]
  struct state {
    int arg;
    int wid;
    int prec;

    bool sign;
    bool space;
    bool zeros;
    bool prefix;
    bool sharp;

    void reset() {
      arg = 0;
      wid = 0;
      prec = 0;

      sign = false;
      space = false;
      zeros = false;
      prefix = false;
      sharp = false;
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
      auto start = it;
      while (is_number(*it)) {
        it = std::next(it);
      }
      prec = std::stoi(std::string(start, it));
    }

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
          format_int(os, val, DEC);
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
    }

    void format_int(std::ostream& os, std::any val, int base = 10) {
      if (val.type() != typeid(int))   {
        throw bad_argument();
      }
      int i = std::any_cast<int>(val);
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
      if (exponent) {
        os << std::scientific << i;
        return ;
      }
      if (percent) {
        os << i*100 << "%";
        return ;
      }
      os << i;
    }

    void format_bool(std::ostream& os, std::any val) {
      if (val.type() != typeid(bool)) {
        throw bad_argument();
      }
      os << std::boolalpha << std::any_cast<bool>(val);
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
        type = "string";
      } else if (val.type() == typeid(char*)) {
        type = "string";
      } else if (val.type() == typeid(int)) {
        type = "int";
      } else if (val.type() == typeid(double)) {
        type = "double";
      } else if (val.type() == typeid(bool)) {
        type = "bool";
      } else if (val.type() == typeid(char)) {
        type = "char";
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

    bool is_number(char c) {
      return c >= '0' && c <= '9';
    }

    void skip_chars(char c, std::string::iterator& start, const std::string::iterator& end) {
      while (start < end && *start == c) {
        start = std::next(start);
      }
    }
  };
}

#endif
