#pragma once

#include "cli/cli.hpp"
#include "strategy.hpp"

namespace strats {
  class AllD : public Strategy {
  public:
    AllD(const cli::Payoffs& payoffs) : Strategy(payoffs) {}

    cli::Strategy getStrat() override { return cli::Strategy::ALLD; }

    Choice getChoice() const override { return Choice::DEFECT; }
    void giveResult(Payoff, Choice) override {}

    ~AllD() override = default;
  };
} // namespace strats
