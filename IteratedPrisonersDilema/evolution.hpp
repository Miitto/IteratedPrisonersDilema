#pragma once

#include "tournament.hpp"
#include <vector>

class Evolution {
  const cli::Args& m_args;

  std::vector<std::vector<double>> m_proportions{};

public:
  Evolution(const cli::Args& args) : m_args(args) {}
  void run();
};