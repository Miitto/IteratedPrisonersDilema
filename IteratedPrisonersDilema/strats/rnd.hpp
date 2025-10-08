#pragma once

#include "../cli/cli.hpp"
#include "strategy.hpp"

#include <random>

namespace strats {
  class Rnd : public Strategy {
    double m_rnd;

    std::mt19937 m_gen;
    std::uniform_real_distribution<double> m_dist{0.0, 1.0};

  public:
    Rnd(const cli::Args& args, double rnd, int seed)
        : Strategy(args), m_rnd(rnd), m_gen(seed) {}

    cli::Strategy getStrat() const override { return cli::Strategy{m_rnd}; }

    Choice getChoice() override {
      return m_dist(m_gen) < m_rnd ? Choice::COOPERATE : Choice::DEFECT;
    }
    void giveResult(Payoff p, Choice) override {}

    ~Rnd() override = default;
  };
} // namespace strats
