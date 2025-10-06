#pragma once

#include "cli/cli.hpp"
#include "strategy.hpp"

namespace strats {
  class AllC : public Strategy {
  public:
    AllC(const cli::Payoffs& payoffs) : Strategy(payoffs) {}

    cli::Strategy getStrat() override { return cli::Strategy::ALLC; }

    Choice getChoice() const override { return Choice::COOPERATE; }
    void giveResult(Payoff, Choice) override {}

    ~AllC() override = default;
  };
} // namespace strats
