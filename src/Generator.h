#pragma once

#include <random>

class Generator {
public:

private:
  std::random_device rd {};
  std::mt19937 generator {rd()};
};
