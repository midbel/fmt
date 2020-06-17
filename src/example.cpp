#include <iostream>
#include "fmt.hpp"

int main(int argc, char* argv[]) {
  try {
    fmt::printf("format namespace: %s", "fmt");
    fmt::printf("version: %d.%d.%d", 0, 0, 1);
    fmt::printf("repository: %s", "https://github.com/midbel/fmt");
  } catch(fmt::fmt_error &e) {
    std::cerr << "misc: " << e.what() << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "unexepcted error" << std::endl;
    return 1;
  }

  try {
    double number = 0.01;
    std::cout << fmt::sprintf("fixed: %f", number) << std::endl;
    std::cout << fmt::sprintf("percent: %p", number) << std::endl;
    std::cout << fmt::sprintf("science: %e", number) << std::endl;
  } catch (fmt::fmt_error &e) {
    std::cerr << "double: " << e.what() << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "unexepcted error" << std::endl;
    return 1;
  }
}
