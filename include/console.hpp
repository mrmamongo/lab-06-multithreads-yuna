//
// Created by lamp on 04.02.2021.
//

#ifndef THREADING_CONSOLE_HPP_
#define THREADING_CONSOLE_HPP_

#include "included_libs.hpp"


namespace hasher {

class console {
 public:
  static std::mutex mutie;
  static po::options_description desc;
  static std::string out_file;
  static size_t threadsCount;
  static std::vector<json> correct_values;
  static void wmain(int argc, char** argv);
  static void initiate();
};
}

#endif  // THREADING_CONSOLE_HPP_
