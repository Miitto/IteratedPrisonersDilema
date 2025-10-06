#pragma once

#include "cli/cli.hpp"
#include "strategy.hpp"

namespace strats {
  class AllC : public Strategy {
  public:
    AllC(const cli::Args& args) : Strategy(args) {}

    cli::Strategy getStrat() override { return cli::Strategy::ALLC; }

    Choice getChoice() override { return Choice::COOPERATE; }
    void giveResult(Payoff, Choice) override {}

    ~AllC() override = default;
  };
} // namespace strats
