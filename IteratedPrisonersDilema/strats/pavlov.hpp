#pragma once

#include "../cli/cli.hpp"
#include "strategy.hpp"

namespace strats {
  /// <summary>
  /// Strategy that starts by cooperating, and afterwards repeats its last move
  /// if it got a reward or punishment, else it swaps its last move.
  /// </summary>
  class Pavlov : public Strategy {
    Choice choice = Choice::COOPERATE;

  public:
    Pavlov(const cli::Args& args) : Strategy(args) {}

    cli::Strategy getStrat() const override { return cli::Strategy::PAVLOV; }
    double getBudget() const override { return 2.0; }

    Choice getChoice() override { return choice; }
    void giveResult(Payoff p, Choice) override {
      switch (p) {
      case Payoff::Temptation:
      case Payoff::Sucker:
        choice = !choice;
        break;
      default: {
        // No-op
      }
      }
    }

    ~Pavlov() override = default;
  };
} // namespace strats
