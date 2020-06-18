#include <iostream>
#include "fmt.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  try {
    fmt::format("format namespace: %s", "fmt");
    fmt::format("version: %d.%d.%d", 0, 0, 1);
    fmt::format("repository: %s", "https://github.com/midbel/fmt");
  } catch(fmt::fmt_error &e) {
    std::cerr << "misc: " << e.what() << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "unexpected error" << std::endl;
    return 1;
  }

  try {
    double number = 0.01;
    std::cout << fmt::sformat("fixed: %f", number) << std::endl;
    std::cout << fmt::sformat("percent: %p", number) << std::endl;
    std::cout << fmt::sformat("science: %e", number) << std::endl;
  } catch (fmt::fmt_error &e) {
    std::cerr << "double: " << e.what() << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "unexpected error" << std::endl;
    return 1;
  }
}
