// Copyright 2020 lamp
#include <iostream>
#include "Hasher.hpp"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

int main() {
  signal(SIGINT, Hasher::sigHandler);
  Hasher::startHashing();
}