#pragma once

#include "cli/cli.hpp"
#include "strategy.hpp"

namespace strats {
  /// <summary>
  /// Strategy that always cooperates
  /// </summary>
  class AllC : public Strategy {
  public:
    AllC(const cli::Args& args) : Strategy(args) {}

    cli::Strategy getStrat() const override { return cli::Strategy::ALLC; }
    double getBudget() const override { return 1.0; }

    Choice getChoice() override { return Choice::COOPERATE; }
    void giveResult(Payoff, Choice) override {}

    ~AllC() override = default;
  };
} // namespace strats
