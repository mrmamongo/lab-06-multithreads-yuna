// Copyright 2020 lamp
#include "Hasher.hpp"

int main() {
  Hasher::Hasher hasher;
  signal(SIGINT, Hasher::sigHandler);
  hasher.startHashing();
}