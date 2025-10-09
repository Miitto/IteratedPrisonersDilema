#pragma once

#include "cli/cli.hpp"
#include <vector>

struct PopDist {
  double proportion;
  uint32_t count;

  PopDist() : proportion(0.0), count(0) {}
  PopDist(double prop, uint32_t totalPop)
      : proportion(prop),
        count(static_cast<uint32_t>(std::round(prop * totalPop))) {}
};

class Evolution {
  const cli::Args& m_args;

  std::vector<std::vector<PopDist>> m_proportions{};

public:
  Evolution(const cli::Args& args) : m_args(args) {}
  void run();

  const std::vector<std::vector<PopDist>>& getProportions() const {
    return m_proportions;
  }

  void printResults(std::ostream& os) const;
};