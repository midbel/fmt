#include <iostream>
#include <exception>
#include "fmt.hpp"

using namespace std;

void format_misc() {
  try {
    fmt::format("format namespace: %s\n", "fmt");
    fmt::format("version: %d.%d.%d\n", 0, 0, 1);
    fmt::format("repository: %s\n", "https://github.com/midbel/fmt");
  } catch(fmt::fmt_error &e) {
    std::cerr << "misc: " << e.what() << std::endl;
  } catch(...) {
    std::cerr << "unexpected error" << std::endl;
  }
}

void format_double() {
  try {
    double number = 0.012345678;
    std::cout << fmt::sformat("fixed: %f", number) << std::endl;
    std::cout << fmt::sformat("fixed: %.2f", number) << std::endl;
    std::cout << fmt::sformat("percent: %p", number) << std::endl;
    std::cout << fmt::sformat("percent: %.2p", number) << std::endl;
    std::cout << fmt::sformat("science: %e", number) << std::endl;
    std::cout << fmt::sformat("fixed+sign: %.2+f", number) << std::endl;
    std::cout << fmt::sformat("percent+sign: %.2+p", number) << std::endl;
    std::cout << fmt::sformat("science+sign: %.2+e", number) << std::endl;
  } catch (fmt::fmt_error &e) {
    std::cerr << "double: " << e.what() << std::endl;
  } catch(exception& e) {
    std::cerr << "unexpected error: " << e.what() << std::endl;
  }
}

void format_int() {
  try {
    fmt::format("binary: %8b\n", 42);
    fmt::format("binary+base: %#8b\n", 42);
    fmt::format("hex: %x\n", 42);
    fmt::format("hexa+base: %#x\n", 42);
    fmt::format("octal: %o\n", 42);
    fmt::format("octal+base: %#o\n", 42);
    fmt::format("decimal: %d\n", 42);
    fmt::format("decimal+base: %#d\n", 42);
    fmt::format("negative: %d\n", -42);
    fmt::format("positive+sign: %+d\n", 42);
  } catch (fmt::fmt_error &e) {
    std::cerr << "integer: " << e.what() << std::endl;
  } catch(exception& e) {
    std::cerr << "unexpected error: " << e.what() << std::endl;
  }
}

void format_bool() {
  try {
    fmt::format("true: %t - %[1]#t (%[1]T)\n", true);
    fmt::format("false: %t - %[1]#t (%[1]T)\n", false);
  } catch (fmt::fmt_error &e) {
    std::cerr << "integer: " << e.what() << std::endl;
  } catch(exception& e) {
    std::cerr << "unexpected error: " << e.what() << std::endl;
  }
}

int main(int argc, char* argv[]) {
  format_misc();
  format_double();
  format_int();
  format_bool();
}
