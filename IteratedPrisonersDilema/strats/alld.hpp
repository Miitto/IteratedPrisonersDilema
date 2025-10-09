#pragma once

#include "cli/cli.hpp"
#include "strategy.hpp"

namespace strats {
  class AllD : public Strategy {
  public:
    AllD(const cli::Args& args) : Strategy(args) {}

    cli::Strategy getStrat() const override { return cli::Strategy::ALLD; }
    double getBudget() const override { return 1.0; }

    Choice getChoice() override { return Choice::DEFECT; }
    void giveResult(Payoff, Choice) override {}

    ~AllD() override = default;
  };
} // namespace strats
