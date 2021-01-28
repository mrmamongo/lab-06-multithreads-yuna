// Copyright 2020 lamp
#include "Hasher.hpp"

int main(int argc, char** argv) {
  Console console(argc, argv);
  signal(SIGINT, Console::Hasher::sigHandler);
  Console::Hasher::startHashing();
}
