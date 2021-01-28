// Copyright 2020 lamp
#include "Hasher.hpp"

int main() {
  Hasher::initiate();
  signal(SIGINT, Hasher::sigHandler);
  Hasher::startHashing();
}
